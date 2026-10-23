# Código fuente

*
  CASCO INTELIGENTE - DETECCION DE CO (prototipo alpha)
  Taller de Diseño en Ingenieria - TEI201
  Placa: ESP32 | Sensor: MQ-135 | Actuadores: LED RGB + buzzer | Conectividad: WiFi

  Flujo: MQ-135 -> ESP32 (procesa) -> LED/buzzer + pagina web

  IMPORTANTE: el MQ-135 NO es un sensor exclusivo de CO (responde tambien a
  amoniaco, alcohol, humo, CO2, etc.). El valor en "ppm" es una ESTIMACION
  usando la curva tipica del MQ-135 para CO. Sirve para el prototipo, no para
  medicion certificada.

  CONEXIONES
    MQ-135  VCC  -> VIN (5V)
    MQ-135  GND  -> GND
    MQ-135  AO   -> resistencia 10k -> GPIO34 -> resistencia 20k -> GND
                    (divisor de voltaje: el ESP32 solo tolera 3.3V)
    LED RGB (catodo comun): R -> 220ohm -> GPIO25
                            G -> 220ohm -> GPIO26
                            B -> 220ohm -> GPIO27
                            pata comun -> GND
    Buzzer activo: + -> GPIO23, - -> GND
*/

#include <WiFi.h>
#include <WebServer.h>

// ==================== CONFIGURACION (EDITAR) ====================
// Red a la que se conecta la placa (usa el hotspot de tu celular).
// Si no logra conectarse en 15 s, crea su propia red WiFi (modo AP).
const char* WIFI_SSID = "TU_RED";
const char* WIFI_PASS = "TU_CLAVE";
const char* AP_SSID   = "Casco-CO";
const char* AP_PASS   = "12345678";   // minimo 8 caracteres

// Pines
const int PIN_MQ     = 34;   // ADC1 (los ADC2 no funcionan con WiFi activo)
const int PIN_LED_R  = 25;
const int PIN_LED_G  = 26;
const int PIN_LED_B  = 27;
const int PIN_BUZZER = 23;

// Electrica del sensor
const float VCC_SENSOR = 5.0;    // voltaje con que alimentas el MQ-135 (V)
const float RL_KOHM    = 10.0;   // resistencia de carga del modulo (kohm), ver nota en la guia
const float DIVISOR    = 1.5;    // (10k + 20k) / 20k  -> si cambias resistencias, recalcula

// Curva del MQ-135 para CO: ppm = A * (Rs/R0)^B
const float CURVA_A = 605.18;
const float CURVA_B = -3.937;
const float RATIO_AIRE_LIMPIO = 3.6;   // Rs/R0 del MQ-135 en aire limpio

// Umbrales (ppm)
const float PPM_PRECAUCION = 20.0;   // amarillo (criterio propio del equipo)
const float PPM_PELIGRO    = 40.0;   // rojo (limite DS 132)

// Tiempos
const unsigned long CALENTAMIENTO_MS   = 60000UL;  // para pruebas 1 min; para la demo usa 180000 (3 min) o mas
const unsigned long PERIODO_LECTURA_MS = 1000UL;   // una lectura por segundo

// ==================== VARIABLES GLOBALES ====================
WebServer server(80);

float R0 = 10.0;             // resistencia del sensor en aire limpio (se calibra)
bool  calibrado = false;
float ppmSuave = 0;          // ppm filtrado
float voltajeSensor = 0;     // voltaje en AO (V)
int   nivel = 0;             // 0=calentando, 1=seguro, 2=precaucion, 3=peligro
unsigned long ultimaLectura = 0;

// ==================== PAGINA WEB ====================
const char PAGINA[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Casco inteligente - CO</title>
<style>
  body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 20px;
         background: #e0e0e0; transition: background 0.5s; }
  h1 { font-size: 22px; }
  #ppm { font-size: 72px; font-weight: bold; margin: 10px 0 0; }
  #unidad { font-size: 20px; }
  #estado { font-size: 28px; font-weight: bold; margin: 20px 0; }
  #alerta { display: none; background: #b71c1c; color: white; padding: 15px;
            border-radius: 8px; font-size: 20px; margin: 10px auto; max-width: 500px; }
  .dato { font-size: 15px; color: #333; }
  button { padding: 10px 18px; font-size: 15px; margin-top: 20px; }
</style>
</head>
<body>
  <h1>Monitoreo de aire (equivalente CO)</h1>
  <div id="ppm">--</div>
  <div id="unidad">ppm (estimado)</div>
  <div id="estado">Conectando...</div>
  <div id="alerta">PELIGRO: concentracion sobre 40 ppm. Evacuar a zona ventilada.</div>
  <div class="dato">Voltaje sensor: <span id="volt">--</span> V</div>
  <button onclick="recalibrar()">Recalibrar en aire limpio</button>
<script>
const colores  = ["#bbdefb", "#c8e6c9", "#fff9c4", "#ffcdd2"];
const mensajes = ["Calentando sensor...", "SEGURO", "PRECAUCION", "PELIGRO"];
async function actualizar() {
  try {
    const r = await fetch('/datos');
    const d = await r.json();
    document.getElementById('ppm').textContent = d.nivel === 0 ? '--' : d.ppm.toFixed(1);
    document.getElementById('volt').textContent = d.voltaje.toFixed(2);
    let txt = mensajes[d.nivel];
    if (d.nivel === 0) txt += " (" + d.restante + " s)";
    document.getElementById('estado').textContent = txt;
    document.body.style.background = colores[d.nivel];
    document.getElementById('alerta').style.display = d.nivel === 3 ? 'block' : 'none';
  } catch (e) {
    document.getElementById('estado').textContent = "Sin conexion con la placa";
  }
}
async function recalibrar() {
  if (confirm("Hazlo solo en aire limpio. Continuar?")) { await fetch('/calibrar'); }
}
setInterval(actualizar, 1000);
actualizar();
</script>
</body>
</html>
)rawliteral";

// ==================== FUNCIONES DEL SENSOR ====================

// Promedia 16 lecturas del ADC y devuelve el voltaje real en AO (antes del divisor)
float leerVoltajeSensor() {
  long suma = 0;
  for (int i = 0; i < 16; i++) {
    suma += analogReadMilliVolts(PIN_MQ);   // ya viene corregido por la calibracion del ESP32
    delay(2);
  }
  float mV = suma / 16.0;
  return (mV / 1000.0) * DIVISOR;
}

// Resistencia del sensor Rs a partir del voltaje de salida (divisor sensor + RL)
float calcularRs(float v) {
  if (v < 0.01) v = 0.01;                          // evita division por cero
  if (v > VCC_SENSOR - 0.01) v = VCC_SENSOR - 0.01;
  return RL_KOHM * (VCC_SENSOR - v) / v;
}

// Calibracion: mide Rs en aire limpio y calcula R0
void calibrar() {
  float suma = 0;
  for (int i = 0; i < 30; i++) {
    suma += calcularRs(leerVoltajeSensor());
    delay(100);
  }
  R0 = (suma / 30.0) / RATIO_AIRE_LIMPIO;
  ppmSuave = 0;          // reinicia el filtro
  calibrado = true;
  Serial.print("Calibrado. R0 = ");
  Serial.print(R0);
  Serial.println(" kohm");
}

// Convierte Rs en ppm estimados con la curva del MQ-135
float calcularPPM(float rs) {
  float ppm = CURVA_A * pow(rs / R0, CURVA_B);
  if (ppm > 9999) ppm = 9999;
  return ppm;
}

int calcularNivel(float ppm) {
  if (ppm >= PPM_PELIGRO)    return 3;
  if (ppm >= PPM_PRECAUCION) return 2;
  return 1;
}

// ==================== ACTUADORES ====================

void setLed(bool r, bool g, bool b) {
  digitalWrite(PIN_LED_R, r);
  digitalWrite(PIN_LED_G, g);
  digitalWrite(PIN_LED_B, b);
}

// Se llama en cada vuelta del loop (sin delay, usa millis para parpadeos)
void actualizarActuadores() {
  unsigned long t = millis();
  switch (nivel) {
    case 0:  // calentando: azul parpadeando
      setLed(0, 0, (t / 500) % 2);
      digitalWrite(PIN_BUZZER, LOW);
      break;
    case 1:  // seguro: verde fijo
      setLed(0, 1, 0);
      digitalWrite(PIN_BUZZER, LOW);
      break;
    case 2:  // precaucion: amarillo (rojo+verde) y pitido corto cada 2 s
      setLed(1, 1, 0);
      digitalWrite(PIN_BUZZER, (t % 2000) < 100);
      break;
    case 3:  // peligro: rojo y pitidos rapidos
      setLed(1, 0, 0);
      digitalWrite(PIN_BUZZER, (t % 400) < 200);
      break;
  }
}

// ==================== SERVIDOR WEB ====================

void handleRaiz() {
  server.send_P(200, "text/html", PAGINA);
}

void handleDatos() {
  long restante = 0;
  if (millis() < CALENTAMIENTO_MS) restante = (CALENTAMIENTO_MS - millis()) / 1000;
  char buf[128];
  snprintf(buf, sizeof(buf),
           "{\"ppm\":%.1f,\"nivel\":%d,\"voltaje\":%.2f,\"restante\":%ld}",
           ppmSuave, nivel, voltajeSensor, restante);
  server.send(200, "application/json", buf);
}

void handleCalibrar() {
  setLed(0, 0, 1);
  digitalWrite(PIN_BUZZER, LOW);
  calibrar();
  server.send(200, "text/plain", "ok");
}

void conectarWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando a WiFi");
  unsigned long inicio = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - inicio < 15000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Conectado. Abre en el navegador: http://");
    Serial.println(WiFi.localIP());
  } else {
    // Plan B: la placa crea su propia red
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASS);
    Serial.print("Sin red. Modo AP '");
    Serial.print(AP_SSID);
    Serial.print("'. Abre: http://");
    Serial.println(WiFi.softAPIP());
  }
}

// ==================== SETUP Y LOOP ====================

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  analogReadResolution(12);                          // 0-4095
  analogSetPinAttenuation(PIN_MQ, ADC_11db);         // rango hasta ~3.1 V

  conectarWiFi();
  server.on("/", handleRaiz);
  server.on("/datos", handleDatos);
  server.on("/calibrar", handleCalibrar);
  server.begin();

  Serial.println("Calentando sensor...");
  Serial.println("tiempo_s,voltaje_V,Rs_kohm,ppm,nivel");   // encabezado para guardar datos como CSV
}

void loop() {
  server.handleClient();
  actualizarActuadores();

  unsigned long ahora = millis();
  if (ahora - ultimaLectura < PERIODO_LECTURA_MS) return;
  ultimaLectura = ahora;

  voltajeSensor = leerVoltajeSensor();

  // Fase 1: calentamiento
  if (ahora < CALENTAMIENTO_MS) {
    nivel = 0;
    return;
  }

  // Fase 2: calibracion automatica (una vez, en aire limpio)
  if (!calibrado) {
    calibrar();
  }

  // Fase 3: medicion normal
  float rs  = calcularRs(voltajeSensor);
  float ppm = calcularPPM(rs);
  ppmSuave = (ppmSuave == 0) ? ppm : (0.7 * ppmSuave + 0.3 * ppm);   // filtro para suavizar
  nivel = calcularNivel(ppmSuave);

  Serial.print(ahora / 1000);   Serial.print(",");
  Serial.print(voltajeSensor);  Serial.print(",");
  Serial.print(rs);             Serial.print(",");
  Serial.print(ppmSuave);       Serial.print(",");
  Serial.println(nivel);
}
