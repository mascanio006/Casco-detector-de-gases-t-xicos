/*
  CAMI - ESP32 clasico + modulo MQ7 + semaforo + BC547/buzzer.
  TEI201. Firmware y dashboard local, sin bibliotecas externas al core ESP32.

  PINES REALES DEL MONTAJE:
    MQ7 AO -> 10k -> union -> GPIO34; union -> 10k -> GND.
    MQ7 VCC -> 5V; MQ7 GND -> GND; DO sin conectar.
    Semaforo G/Y/R -> GPIO25/26/27; GND comun. Resistencias ya en el modulo.
    GPIO23 -> 1k -> base BC547; emisor -> GND; colector -> negro buzzer.
    Rojo de un buzzer especificado para 5V -> 5V despues del interruptor.

  Mide cambios de senal, NO ppm ni porcentaje de CO. No controla el ciclo
  termico del MQ7. Los 60 s de inicio son una espera de demostracion y NO
  sustituyen el acondicionamiento/calibracion del sensor ni certifican aire.
  NO usar como detector de seguridad personal.
*/
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <LittleFS.h>
#include <time.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
// ===== logica.h integrado =====

// Logica comun del firmware y de las comprobaciones de escritorio.
// Los umbrales son aumentos de ADC, NO concentraciones de CO.
enum Estado { NORMAL = 0, PREALERTA = 1, ALERTA = 2 };
constexpr int AUMENTO_AMARILLO = 120;
constexpr int AUMENTO_ROJO = 300;
constexpr int HISTERESIS = 30;

inline Estado siguienteEstado(Estado anterior, float aumento) {
  switch (anterior) {
    case NORMAL:
      if (aumento >= AUMENTO_ROJO) return ALERTA;
      if (aumento >= AUMENTO_AMARILLO) return PREALERTA;
      return NORMAL;
    case PREALERTA:
      if (aumento >= AUMENTO_ROJO) return ALERTA;
      if (aumento < AUMENTO_AMARILLO - HISTERESIS) return NORMAL;
      return PREALERTA;
    case ALERTA:
      if (aumento < AUMENTO_ROJO - HISTERESIS)
        return aumento >= AUMENTO_AMARILLO ? PREALERTA : NORMAL;
      return ALERTA;
  }
  return NORMAL;
}

inline float filtrar(float previo, float nuevo) {
  return 0.7f * previo + 0.3f * nuevo;
}

inline bool adcEnRango(int valor) {
  // Solo detecta senal pegada a los extremos. NO verifica que haya sensor,
  // ni detecta todas las desconexiones, fallas de alimentacion o deriva.
  return valor > 5 && valor < 4090;
}

// ===== dashboard.h integrado =====
// Pagina autocontenida: no necesita internet, fuentes externas ni Chart.js.
const char DASHBOARD[] PROGMEM = R"CAMIHTML(<!doctype html>
<html lang="es"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>CAMI · Monitor MQ7</title><style>
:root{--ink:#152c35;--muted:#58707a;--line:#dce7e9;--teal:#006d70;--bg:#f0f5f5;--red:#bb3043;--amber:#a56a00;--green:#08784f}
*{box-sizing:border-box}body{margin:0;background:var(--bg);color:var(--ink);font:16px system-ui,-apple-system,Segoe UI,sans-serif}main{max-width:1180px;margin:auto;padding:28px 22px 48px}header{display:flex;justify-content:space-between;align-items:center;gap:16px;margin-bottom:22px}.brand{font-weight:900;font-size:38px;letter-spacing:-2px}.eyebrow{font-size:12px;letter-spacing:1.5px;text-transform:uppercase;color:var(--muted);font-weight:750}h1{font-size:24px;margin:5px 0 0}h2{font-size:18px;margin:0 0 16px}p{line-height:1.5}.sub,.small{font-size:13px;color:var(--muted);line-height:1.5}.pill{display:inline-flex;align-items:center;gap:8px;border:1px solid var(--line);padding:8px 12px;border-radius:30px;background:white;font-size:13px}.dot{width:9px;height:9px;border-radius:50%;background:var(--muted)}.live .dot{background:var(--green)}.stale .dot{background:var(--red)}.status{background:var(--ink);color:white;border-radius:18px;padding:22px;display:grid;grid-template-columns:1fr auto;gap:16px;margin-bottom:18px}.status .eyebrow{color:#becfd3}.status h2{font-size:28px;margin:5px 0 8px}.status p{margin:0;color:#dce7e9;max-width:770px}.status.normal{background:#125c4c}.status.prealerta,.status.inicio{background:#705020}.status.alerta,.status.fallo{background:#862b3a}.lamps{display:flex;align-items:center;gap:10px}.lamp{width:23px;height:23px;border-radius:50%;background:#ffffff22;border:2px solid #ffffff44}.lamp.g.on{background:#5ee4a0}.lamp.y.on{background:#ffd257}.lamp.r.on{background:#ff7a87}.metrics{display:grid;grid-template-columns:repeat(4,1fr);gap:14px;margin-bottom:18px}.card{background:white;border:1px solid var(--line);border-radius:16px;padding:20px;min-width:0}.value{font-size:34px;font-weight:800;letter-spacing:-1px;margin:9px 0}.unit{font-size:16px;font-weight:500;color:var(--muted)}.grid{display:grid;grid-template-columns:1.7fr 1fr;gap:18px}.chart-wrap{position:relative;width:100%;height:285px}canvas{width:100%;height:100%;display:block}.chart-head{display:flex;align-items:center;justify-content:space-between;gap:10px;flex-wrap:wrap;margin-bottom:12px}.chart-head h2{margin:0}.legend{display:flex;gap:15px;flex-wrap:wrap;font-size:12px;margin-top:10px}.legend span:before{content:'';display:inline-block;width:16px;height:3px;background:var(--teal);margin:0 6px 3px 0}.legend .yellow:before{background:#b57900}.legend .red:before{background:var(--red)}.rows{display:grid;grid-template-columns:1fr auto;gap:11px 16px;font-size:14px}.rows>span{color:var(--muted)}.rows strong{text-align:right;overflow-wrap:anywhere;max-width:230px}.rule{border-top:1px solid var(--line);margin:18px 0}.controls{display:flex;gap:10px;flex-wrap:wrap;margin-top:18px}button,.button{display:inline-block;cursor:pointer;text-decoration:none;background:var(--teal);color:white;border:0;border-radius:9px;padding:11px 14px;font:600 14px system-ui}button.secondary,.button.secondary{background:white;color:var(--teal);border:1px solid #a9c4c5}button:disabled{opacity:.45;cursor:wait}select{font:14px system-ui;border:1px solid var(--line);border-radius:8px;padding:8px;background:white;color:var(--ink)}.notice{margin-top:18px;border-left:4px solid #ba840f;background:#fff7df;padding:16px 18px;border-radius:8px;font-size:13px;line-height:1.6}.notice strong{display:block;margin-bottom:4px}#message{min-height:22px;font-size:13px;margin-top:12px;color:var(--muted)}.lower{margin-top:18px}.table-scroll{overflow:auto;max-height:250px}table{border-collapse:collapse;width:100%;font-size:13px}td,th{text-align:left;padding:10px;border-bottom:1px solid var(--line);white-space:nowrap}th{color:var(--muted)}footer{margin-top:20px;font-size:12px;color:var(--muted)}.tag{font-weight:650;font-size:12px}a{color:var(--teal)}[hidden]{display:none!important}@media(max-width:850px){.metrics{grid-template-columns:repeat(2,1fr)}.grid{grid-template-columns:1fr}.status{grid-template-columns:1fr}.lamps{justify-content:flex-start}.brand{font-size:32px}.value{font-size:29px}}@media(max-width:430px){main{padding:18px 12px}.card{padding:15px}header{align-items:flex-start}.pill{font-size:11px;padding:7px}.value{font-size:26px}.status h2{font-size:24px}}
</style></head><body><main>
<header><div><div class="brand">CAMI<span style="color:var(--teal)">.</span></div><div class="sub">Casco · Prototipo IoT · MQ7 + ESP32</div></div><div id="connection" class="pill stale"><span class="dot"></span><span id="connectionText">Conectando al casco…</span></div></header>
<div class="eyebrow">Monitoreo de la señal del sensor</div><h1>Una lectura, las mismas alertas.</h1><p class="sub">Datos reales del ESP32. Muestreo de señal a 10 Hz · pantalla cada segundo.</p>
<section id="status" class="status inicio"><div><div class="eyebrow">Estado del dispositivo</div><h2 id="stateTitle">Esperando datos</h2><p id="action">Conecta este equipo a la red Wi-Fi CAMI y abre 192.168.4.1.</p></div><div class="lamps" aria-label="Salidas ordenadas por el firmware"><span id="lampG" class="lamp g" title="Verde"></span><span id="lampY" class="lamp y" title="Amarillo"></span><span id="lampR" class="lamp r" title="Rojo"></span><span id="buzzer" class="small" style="color:white">Buzzer —</span></div></section>
<section class="metrics">
<div class="card"><div class="eyebrow">Voltaje AO estimado</div><div class="value"><span id="voltage">—</span> <span class="unit">V</span></div><div class="small">Reconstruido con divisor 10 kΩ / 10 kΩ</div></div>
<div class="card"><div class="eyebrow">Señal filtrada</div><div class="value"><span id="adc">—</span> <span class="unit">ADC</span></div><div class="small">Escala de 12 bits: 0–4095</div></div>
<div class="card"><div class="eyebrow">Aumento de señal</div><div class="value"><span id="percent">—</span> <span class="unit">%</span></div><div class="small">Cambio del ADC respecto de la referencia; no es % de CO</div></div>
<div class="card"><div class="eyebrow">Entradas en rojo</div><div class="value" id="alerts">0</div><div class="small">En esta sesión; excluye prueba de actuadores</div></div>
</section>
<section class="grid"><div class="card"><div class="chart-head"><h2>Tendencia del aumento</h2><select id="historyMode" aria-label="Origen del gráfico"><option value="live">En vivo · últimos 3 min</option><option value="saved">Historial guardado</option></select></div><div class="small" id="chartInfo">Aumento respecto de la referencia, en unidades ADC.</div><div class="chart-wrap"><canvas id="chart" aria-label="Gráfico de aumento ADC con umbrales amarillo y rojo"></canvas></div><div class="legend"><span>Aumento ADC</span><span class="yellow">Amarillo: +120</span><span class="red">Rojo: +300</span></div><div class="small" id="stats">Esperando muestras…</div></div>
<div class="card"><h2>Lectura y conectividad</h2><div class="rows"><span>Referencia de esta sesión</span><strong id="reference">—</strong><span>Aumento absoluto</span><strong id="delta">—</strong><span>Voltaje en GPIO34</span><strong id="pinVoltage">—</strong><span>Calidad de lectura</span><strong id="quality">—</strong><span>Tiempo encendido</span><strong id="uptime">—</strong><span>Wi-Fi con internet</span><strong id="wifi">—</strong><span>ThingSpeak · canal 3571982</span><strong id="cloud">—</strong><span>Último envío aceptado</span><strong id="lastCloud">—</strong><span>Registro en memoria</span><strong id="storage">—</strong><span>Última muestra recibida</span><strong id="sampleAge">—</strong></div><div class="rule"></div><div class="small">ThingSpeak: campo 1 = voltaje AO estimado; campo 2 = ADC filtrado; campo 3 = alerta (0, 1, 2). Envío cada 20 s, sin pruebas de luces ni datos inválidos.</div></div></section>
<section class="card lower"><h2>Control y evidencia</h2><div class="small">El test enciende cada luz y prueba el buzzer. Se identifica como prueba y no simula una lectura de gas.</div><div class="controls"><button id="testButton">Probar luces y buzzer</button><button class="secondary" id="referenceButton">Tomar nueva referencia</button><a class="button secondary" href="/registro.csv" download>Descargar CSV actual</a><a class="button secondary" href="/anterior.csv" download>Descargar CSV anterior</a><button class="secondary" id="formatButton" hidden>Inicializar registro</button></div><div id="message" role="status"></div></section>
<section class="card lower"><h2>Últimas muestras recibidas</h2><div class="table-scroll"><table><thead><tr><th>Tiempo de sesión</th><th>AO estimado (V)</th><th>ADC</th><th>Aumento ADC</th><th>Estado medido</th></tr></thead><tbody id="samples"><tr><td colspan="5">Sin muestras todavía.</td></tr></tbody></table></div></section>
<aside class="notice"><strong>Prototipo experimental · no mide ppm ni porcentaje de CO</strong>El aumento del voltaje del MQ7 activa umbrales de demostración. No identifica inequívocamente CO ni garantiza aire seguro. La referencia inicial no es una calibración de concentración. El montaje actual no controla el ciclo térmico del MQ7; este sistema no sustituye un detector de CO certificado.</aside>
<footer>Los indicadores de luces y buzzer muestran las órdenes del firmware, no una verificación eléctrica de los componentes. Registros locales con rotación: descarga la evidencia antes de que se sobrescriba.</footer>
</main><script>
'use strict';
const $=id=>document.getElementById(id);let live=[],saved=[],lastGood=0,lastSession='',lastSeq=-1,lastData=null;
const num=(v,d=1)=>typeof v==='number'&&Number.isFinite(v)?v.toFixed(d):'—';
const elapsed=ms=>{const s=Math.floor(ms/1000);return `${Math.floor(s/60)} min ${s%60} s`;};
function message(t){$('message').textContent=t;}
async function request(url,options={}){const controller=new AbortController();const timer=setTimeout(()=>controller.abort(),options.timeout||5000);try{const r=await fetch(url,{...options,cache:'no-store',signal:controller.signal});if(!r.ok){let text=await r.text();try{text=JSON.parse(text).mensaje||text;}catch(_){}throw new Error(text||`HTTP ${r.status}`);}return r;}finally{clearTimeout(timer);}}
function render(d){lastData=d;lastGood=Date.now();$('connection').className='pill live';$('connectionText').textContent='Casco conectado';
 const title={inicio:'Estabilizando señal',referencia:'Tomando referencia',normal:'Señal bajo el umbral',prealerta:'Aumento de señal',alerta:'Alerta por aumento',fallo:'Señal fuera de rango'};
 const action={inicio:`Espera inicial: ${d.restante_s} s. La lectura continúa; las alertas aún no están habilitadas.`,referencia:`Tomando 50 muestras de referencia: ${d.referencia_muestras}/50.`,normal:'Sin aumento que supere los umbrales experimentales. Esto no certifica aire seguro.',prealerta:'La señal aumentó al menos 120 unidades ADC. Revisa la tendencia; no equivale a un límite de CO.',alerta:'Aumento de señal sobre el umbral experimental: rojo y buzzer activados. Una evaluación real requiere un detector certificado.',fallo:'El ADC está cerca de un extremo. Revisa alimentación, AO, divisor y conexiones. La falla no identifica por sí sola su causa.'};
 const mode=d.prueba?'inicio':d.fase;$('status').className='status '+mode;$('stateTitle').textContent=d.prueba?'Prueba de luces y buzzer':title[d.fase];$('action').textContent=d.prueba?'Prueba manual en curso. La captura del sensor continúa. No se registra como presencia de gas.':action[d.fase];
 for(const [id,on] of [['lampG',d.salidas.verde],['lampY',d.salidas.amarillo],['lampR',d.salidas.rojo]])$(id).classList.toggle('on',on);
 $('buzzer').textContent=d.salidas.buzzer?'Buzzer ON':'Buzzer OFF';$('voltage').textContent=num(d.voltaje_ao_v,3);$('adc').textContent=num(d.adc,0);$('percent').textContent=num(d.cambio_pct,1);$('alerts').textContent=d.alertas;
 $('reference').textContent=d.referencia_adc===null?'Pendiente':num(d.referencia_adc,0)+' ADC';$('delta').textContent=d.aumento_adc===null?'—':num(d.aumento_adc,0)+' ADC';$('pinVoltage').textContent=num(d.voltaje_pin_v,3)+' V';$('quality').textContent=d.calidad;$('uptime').textContent=elapsed(d.tiempo_ms);
 $('wifi').textContent=d.wifi_conectado?'Red enlazada · '+d.ip_sta:'Sin enlace · dashboard local activo';$('cloud').textContent=d.nube.estado;$('lastCloud').textContent=d.nube.envios?`${d.nube.envios} envíos · hace ${Math.floor(d.nube.edad_ms/1000)} s`:'Sin envío confirmado';
 $('storage').textContent=d.almacenamiento.disponible?(d.almacenamiento.error?'Error al escribir':`${Math.round(d.almacenamiento.usado/1024)} / ${Math.round(d.almacenamiento.total/1024)} kB`):'Sin montar · inicializar';$('formatButton').hidden=d.almacenamiento.disponible;
 $('testButton').disabled=d.prueba||!d.listo||d.estado===2||!d.senal_valida;$('referenceButton').disabled=d.prueba||!d.listo||d.estado!==0||!d.senal_valida;
 if(lastSession!==d.sesion){live=[];lastSeq=-1;lastSession=d.sesion;}
 if(d.secuencia!==lastSeq){lastSeq=d.secuencia;live.push(d);if(live.length>180)live.shift();}
 draw();renderTable();}
function renderTable(){const rows=live.slice(-8).reverse();$('samples').innerHTML=rows.map(d=>`<tr><td>${elapsed(d.tiempo_ms)}</td><td>${num(d.voltaje_ao_v,3)}</td><td>${num(d.adc,0)}</td><td>${num(d.aumento_adc,0)}</td><td>${d.prueba?'Prueba manual':d.fase}</td></tr>`).join('');}
function draw(){const data=$('historyMode').value==='live'?live:saved;const canvas=$('chart'),ctx=canvas.getContext('2d');const box=canvas.getBoundingClientRect(),dpr=window.devicePixelRatio||1;canvas.width=Math.round(box.width*dpr);canvas.height=Math.round(box.height*dpr);ctx.scale(dpr,dpr);const W=box.width,H=box.height,p={l:47,r:15,t:23,b:35};const usable=data.filter(d=>Number.isFinite(d.aumento_adc));const vals=usable.map(d=>d.aumento_adc);const lo=Math.min(-30,...vals),hi=Math.max(360,...vals);const y=v=>p.t+(hi-v)/(hi-lo)*(H-p.t-p.b);const x=i=>p.l+(data.length<2?0:i/(data.length-1))*(W-p.l-p.r);ctx.clearRect(0,0,W,H);ctx.font='11px system-ui';ctx.textAlign='right';ctx.fillStyle='#58707a';for(let i=0;i<5;i++){const v=lo+(hi-lo)*i/4;ctx.strokeStyle='#e7eeee';ctx.beginPath();ctx.moveTo(p.l,y(v));ctx.lineTo(W-p.r,y(v));ctx.stroke();ctx.fillText(Math.round(v),p.l-7,y(v)+4);}for(const [v,col] of [[120,'#b57900'],[300,'#bb3043']]){ctx.strokeStyle=col;ctx.setLineDash([5,5]);ctx.beginPath();ctx.moveTo(p.l,y(v));ctx.lineTo(W-p.r,y(v));ctx.stroke();}ctx.setLineDash([]);ctx.strokeStyle='#006d70';ctx.lineWidth=2;ctx.beginPath();let pen=false,session='';for(let i=0;i<data.length;i++){const d=data[i];if(!Number.isFinite(d.aumento_adc)){pen=false;continue;}if(!pen||session!==d.sesion)ctx.moveTo(x(i),y(d.aumento_adc));else ctx.lineTo(x(i),y(d.aumento_adc));pen=true;session=d.sesion;}ctx.stroke();ctx.fillStyle='#58707a';ctx.textAlign='left';if(data.length){ctx.fillText(elapsed(data[0].tiempo_ms),p.l,H-10);ctx.textAlign='right';ctx.fillText(elapsed(data[data.length-1].tiempo_ms),W-p.r,H-10);}if(!vals.length){ctx.textAlign='center';ctx.fillText('El gráfico comienza al obtener la referencia.',W/2,H/2);}const sum=vals.reduce((a,b)=>a+b,0);$('stats').textContent=vals.length?`${vals.length} muestras · media ${num(sum/vals.length,0)} ADC · máximo ${num(Math.max(...vals),0)} ADC`:'Esperando referencia válida…';}
async function poll(){try{const r=await request('/datos');render(await r.json());}catch(e){$('connection').className='pill stale';$('connectionText').textContent='Sin conexión al casco';}finally{setTimeout(poll,1000);}}
setInterval(()=>{if(lastGood){const seconds=Math.floor((Date.now()-lastGood)/1000);$('sampleAge').textContent=seconds+' s';if(seconds>5){$('connection').className='pill stale';$('connectionText').textContent='Datos antiguos · reconectar';$('stateTitle').textContent='Sin datos recientes';$('action').textContent='La pantalla conserva la última lectura, pero no confirma el estado actual. Revisa Wi-Fi y alimentación.';}}},1000);
$('testButton').onclick=async()=>{try{const r=await request('/prueba',{method:'POST'});message((await r.json()).mensaje);}catch(e){message(e.message);}};
$('referenceButton').onclick=async()=>{if(!confirm('¿Tomar una referencia nueva? Hazlo solo en un entorno de referencia conocido, sin estímulos de prueba. Esto no calibra ppm ni certifica aire limpio.'))return;try{const r=await request('/referencia',{method:'POST'});message((await r.json()).mensaje);}catch(e){message(e.message);}};
$('formatButton').onclick=async()=>{if(!confirm('Inicializar la memoria de registros borrará los CSV locales que pudiera contener. ¿Continuar?'))return;try{const r=await request('/inicializar',{method:'POST',timeout:20000});message((await r.json()).mensaje);}catch(e){message(e.message);}};
$('historyMode').onchange=async()=>{if($('historyMode').value==='live'){$('chartInfo').textContent='Aumento respecto de la referencia, en unidades ADC.';draw();return;}message('Leyendo historial guardado…');try{saved=[];for(const url of ['/anterior.csv','/registro.csv']){const r=await fetch(url,{cache:'no-store'});if(r.status===404)continue;if(!r.ok)throw new Error('No se pudo leer el registro');const text=await r.text();const lines=text.trim().split(/\r?\n/);for(const line of lines.slice(1)){const c=line.split(',');if(c.length!==11)continue;saved.push({sesion:c[0],tiempo_ms:Number(c[1]),voltaje_ao_v:Number(c[3]),adc:Number(c[4]),aumento_adc:c[6]===''?null:Number(c[6]),fase:c[8]});}}saved=saved.slice(-300);$('chartInfo').textContent='Hasta 300 registros persistentes. El tiempo se reinicia con cada sesión; las sesiones no se unen con líneas.';message(saved.length?`Historial: ${saved.length} registros cargados.`:'Todavía no hay registros guardados.');draw();}catch(e){message(e.message);}};
window.addEventListener('resize',draw);poll();
</script></body></html>)CAMIHTML";


// CONFIGURACION: completar Wi-Fi y clave ThingSpeak SOLO en copia local.
// NO subir credenciales reales a repositorios publicos.
const char WIFI_SSID[] = "TU_WIFI";
const char WIFI_CLAVE[] = "TU_CLAVE_WIFI";
// La red debe ser 2.4 GHz y no requerir iniciar sesion en un portal web.
const char AP_NOMBRE[] = "CAMI";
const char AP_CLAVE[] = "CAMI12345";
const char THINGSPEAK_WRITE_KEY[] = "REEMPLAZAR_EN_COPIA_LOCAL";
const unsigned long THINGSPEAK_CHANNEL = 3571982;

const uint8_t PIN_SENSOR = 34;
const uint8_t PIN_BUZZER = 23;
const uint8_t LED_VERDE = 25;
const uint8_t LED_AMARILLO = 26;
const uint8_t LED_ROJO = 27;
const unsigned int FRECUENCIA_BUZZER = 2048;
// false conserva tone(2048 Hz) del programa anterior, para buzzer pasivo.
// Si el componente es un buzzer ACTIVO de 5V, usar true: activacion continua.
const bool BUZZER_ACTIVO = false;

const uint32_t ESPERA_INICIAL_MS = 60000;
const uint32_t PERIODO_MUESTRA_MS = 100;  // 10 Hz; no es el tiempo de respuesta del gas.
const uint32_t PERIODO_HISTORIAL_MS = 1000;
const uint32_t PERIODO_REGISTRO_MS = 5000;
const uint32_t PERIODO_NUBE_MS = 20000;
const uint16_t MUESTRAS_REFERENCIA = 50;  // 50 x 100 ms = unos 5 s.
const float FACTOR_DIVISOR = 2.0f;       // SOLO para divisor 10k / 10k.

enum Fase { INICIO, REFERENCIA, MONITOREO };
struct Lectura { int adc; float milivoltios; };
struct MuestraNube {
  float voltaje;
  int adc;
  int estado;
  uint32_t tomadaMs;
};
struct ResumenNube {
  int resultado;  // 0 espera, 1 aceptado, 2 sin WiFi, 3 error, 4 enviando, 5 sin tarea
  int http;
  uint32_t envios;
  uint32_t ultimoExitoMs;
  uint32_t entrada;
};
// Prototipos explicitos: evita que Arduino los inserte antes de estos tipos.
Lectura leerSensor();
ResumenNube copiarNube();

WebServer servidor(80);
QueueHandle_t colaNube = nullptr;
portMUX_TYPE mutexNube = portMUX_INITIALIZER_UNLOCKED;
ResumenNube nube = {0, 0, 0, 0, 0};
Fase fase = INICIO;
Estado estado = NORMAL;
float lecturaFiltrada = 0, mvFiltrados = 0, referencia = 0;
float sumaReferencia = 0;
uint16_t muestrasReferencia = 0;
int adcActual = 0;
bool tieneMuestra = false, referenciaLista = false, senalValida = false;
bool falloSenal = false, pruebaActiva = false, almacenamientoOK = false;
bool errorRegistro = false, relojSolicitado = false;
uint8_t malas = 0, buenas = 0;
bool salidaVerde = false, salidaAmarillo = false, salidaRojo = false;
bool buzzerEncendido = false;
uint32_t inicioMs = 0, ultimaMuestraMs = 0, ultimoHistorialMs = 0;
uint32_t ultimoRegistroMs = 0, ultimoEnvioMs = 0, ultimoWiFiMs = 0;
uint32_t ultimoSerialMs = 0, inicioPruebaMs = 0, secuencia = 0, alertas = 0;
uint32_t limiteArchivo = 0;
char sesion[9];
int ultimaMarcaRegistro = -999;
const char ARCHIVO[] = "/registro.csv";
const char ANTERIOR[] = "/anterior.csv";
const char CABECERA[] = "sesion,tiempo_ms,fecha_utc,voltaje_ao_v,adc_filtrado,referencia_adc,aumento_adc,cambio_adc_pct,fase,estado,prueba\n";

bool wifiConfigurado() {
  return strlen(WIFI_SSID) && strcmp(WIFI_SSID, "TU_WIFI") != 0;
}
const char* nombreFase() {
  if (fase == INICIO) return "inicio";
  if (fase == REFERENCIA) return "referencia";
  if (falloSenal) return "fallo";
  if (estado == PREALERTA) return "prealerta";
  if (estado == ALERTA) return "alerta";
  return "normal";
}
float aumentoADC() { return lecturaFiltrada - referencia; }
float porcentajeADC() { return referencia > 0 ? 100.0f * aumentoADC() / referencia : 0; }
float voltajeAO() { return mvFiltrados * FACTOR_DIVISOR / 1000.0f; }

Lectura leerSensor() {
  uint32_t sumaADC = 0, sumaMV = 0;
  // Ventana corta de 20 conversiones. analogRead y analogReadMilliVolts son
  // conversiones consecutivas, no la misma conversion. Ambas se promedian.
  for (uint8_t i = 0; i < 20; ++i) {
    sumaADC += analogRead(PIN_SENSOR);
    sumaMV += analogReadMilliVolts(PIN_SENSOR);
    delayMicroseconds(100);
  }
  return {int(sumaADC / 20), sumaMV / 20.0f};
}

void activarBuzzer(bool encender) {
  if (encender == buzzerEncendido) return;
  if (BUZZER_ACTIVO) digitalWrite(PIN_BUZZER, encender ? HIGH : LOW);
  else if (encender) tone(PIN_BUZZER, FRECUENCIA_BUZZER);
  else { noTone(PIN_BUZZER); digitalWrite(PIN_BUZZER, LOW); }
  buzzerEncendido = encender;
}

void escribirSalidas(bool verde, bool amarillo, bool rojo, bool sonido) {
  digitalWrite(LED_VERDE, verde ? HIGH : LOW);
  digitalWrite(LED_AMARILLO, amarillo ? HIGH : LOW);
  digitalWrite(LED_ROJO, rojo ? HIGH : LOW);
  salidaVerde = verde; salidaAmarillo = amarillo; salidaRojo = rojo;
  activarBuzzer(sonido);
}

void actualizarSalidas(uint32_t ahora) {
  // La prueba manual nunca sustituye una alerta ya detectada.
  if (pruebaActiva && (falloSenal || (referenciaLista && estado == ALERTA)))
    pruebaActiva = false;
  if (pruebaActiva) {
    uint32_t t = ahora - inicioPruebaMs;
    if (t < 1000) { escribirSalidas(true, false, false, false); return; }
    if (t < 2000) { escribirSalidas(false, true, false, false); return; }
    if (t < 3000) { escribirSalidas(false, false, true, true); return; }
    pruebaActiva = false;
  }
  if (fase != MONITOREO || falloSenal) {
    escribirSalidas(false, ((ahora / 250) % 2) != 0, false, false);
    return;
  }
  escribirSalidas(estado == NORMAL, estado == PREALERTA, estado == ALERTA,
                  estado == ALERTA);
}

void procesarSensor(uint32_t ahora) {
  if (ahora - ultimaMuestraMs < PERIODO_MUESTRA_MS) return;
  ultimaMuestraMs = ahora;
  Lectura l = leerSensor();
  adcActual = l.adc;
  senalValida = adcEnRango(l.adc);
  if (!tieneMuestra) {
    lecturaFiltrada = l.adc; mvFiltrados = l.milivoltios; tieneMuestra = true;
  } else {
    lecturaFiltrada = filtrar(lecturaFiltrada, l.adc);
    mvFiltrados = filtrar(mvFiltrados, l.milivoltios);
  }
  if (senalValida) {
    malas = 0; if (buenas < 5) ++buenas;
    if (buenas >= 5) falloSenal = false;
  } else {
    buenas = 0; if (malas < 3) ++malas;
    if (malas >= 3) falloSenal = true;
  }
  if (fase == INICIO && ahora - inicioMs >= ESPERA_INICIAL_MS) {
    fase = REFERENCIA; sumaReferencia = 0; muestrasReferencia = 0;
    Serial.println("Tomando referencia: 50 muestras validas, unos 5 segundos.");
  }
  if (fase == REFERENCIA) {
    if (!senalValida || falloSenal) { sumaReferencia = 0; muestrasReferencia = 0; return; }
    sumaReferencia += l.adc;
    if (++muestrasReferencia >= MUESTRAS_REFERENCIA) {
      referencia = sumaReferencia / MUESTRAS_REFERENCIA;
      lecturaFiltrada = referencia;
      referenciaLista = true; fase = MONITOREO; estado = NORMAL;
      Serial.printf("Referencia ADC: %.1f. Sistema listo (sin ppm).\n", referencia);
    }
    return;
  }
  if (fase != MONITOREO || !senalValida || falloSenal) return;
  Estado nuevo = siguienteEstado(estado, aumentoADC());
  if (nuevo == ALERTA && estado != ALERTA) ++alertas;
  estado = nuevo;
}

void establecerNube(int resultado, int http = 0, uint32_t entrada = 0) {
  portENTER_CRITICAL(&mutexNube);
  nube.resultado = resultado; nube.http = http;
  if (resultado == 1) {
    ++nube.envios; nube.ultimoExitoMs = millis(); nube.entrada = entrada;
  }
  portEXIT_CRITICAL(&mutexNube);
}
ResumenNube copiarNube() {
  portENTER_CRITICAL(&mutexNube);
  ResumenNube copia = nube;
  portEXIT_CRITICAL(&mutexNube);
  return copia;
}
const char* textoNube(int r) {
  if (!wifiConfigurado()) return "Sin configurar Wi-Fi de internet";
  switch (r) {
    case 1: return "Ultimo envio aceptado";
    case 2: return "Sin Wi-Fi; registro local activo si esta inicializado";
    case 3: return "Envio rechazado o error de conexion";
    case 4: return "Enviando";
    case 5: return "No se pudo crear la tarea de envio";
    default: return "Esperando referencia y primera muestra";
  }
}

void tareaThingSpeak(void*) {
  MuestraNube dato;
  for (;;) {
    if (xQueueReceive(colaNube, &dato, portMAX_DELAY) != pdTRUE) continue;
    if (WiFi.status() != WL_CONNECTED) { establecerNube(2); continue; }
    if (millis() - dato.tomadaMs > 5000) continue;  // No enviar un dato viejo.
    establecerNube(4);
    WiFiClientSecure cliente;
    // Prototipo: TLS cifrado sin autenticar el certificado del servidor.
    // Antes de un despliegue real, cargar una CA valida con setCACert().
    cliente.setInsecure();
    cliente.setHandshakeTimeout(5);
    HTTPClient http;
    http.setConnectTimeout(3000); http.setTimeout(3000);
    int codigo = -1;
    long entrada = 0;
    if (http.begin(cliente, "https://api.thingspeak.com/update")) {
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String cuerpo = "api_key=" + String(THINGSPEAK_WRITE_KEY);
      cuerpo += "&field1=" + String(dato.voltaje, 3);
      cuerpo += "&field2=" + String(dato.adc);
      cuerpo += "&field3=" + String(dato.estado);
      codigo = http.POST(cuerpo);
      if (codigo == 200) entrada = http.getString().toInt();
      http.end();
    }
    // HTTP 200 con cuerpo 0 es un fallo, NO una confirmacion de guardado.
    establecerNube(codigo == 200 && entrada > 0 ? 1 : 3, codigo, entrada > 0 ? entrada : 0);
  }
}

void atenderRed(uint32_t ahora) {
  if (!wifiConfigurado()) return;
  if (WiFi.status() == WL_CONNECTED) {
    if (!relojSolicitado) {
      configTime(0, 0, "pool.ntp.org", "time.google.com");
      relojSolicitado = true;
      Serial.print("IP en red de internet: "); Serial.println(WiFi.localIP());
    }
  } else if (ahora - ultimoWiFiMs >= 30000) {
    ultimoWiFiMs = ahora; WiFi.reconnect();
  }
  if (!colaNube || !referenciaLista || !senalValida || falloSenal || pruebaActiva) return;
  if (ahora - ultimoEnvioMs < PERIODO_NUBE_MS) return;
  ultimoEnvioMs = ahora;
  MuestraNube dato = {voltajeAO(), int(lroundf(lecturaFiltrada)), int(estado), ahora};
  // Cola de un elemento: conserva la muestra mas reciente. El HTTP no corre
  // en loop(), por lo que una caida de internet no espera en la alarma local.
  xQueueOverwrite(colaNube, &dato);
}

void actualizarLimite() {
  limiteArchivo = (uint32_t)(LittleFS.totalBytes() / 3);
  if (limiteArchivo > 256UL * 1024) limiteArchivo = 256UL * 1024;
}
bool crearRegistro() {
  if (LittleFS.exists(ARCHIVO)) return true;
  File f = LittleFS.open(ARCHIVO, "w");
  if (!f) return false;
  bool ok = f.print(CABECERA) == strlen(CABECERA); f.close(); return ok;
}
bool rotarRegistro() {
  if (LittleFS.exists(ANTERIOR) && !LittleFS.remove(ANTERIOR)) return false;
  if (LittleFS.exists(ARCHIVO) && !LittleFS.rename(ARCHIVO, ANTERIOR)) return false;
  return crearRegistro();
}
String fechaUTC() {
  time_t ahora = time(nullptr);
  if (ahora < 1700000000) return "";  // Sin NTP: sesion + uptime siguen siendo validos.
  struct tm utc; gmtime_r(&ahora, &utc);
  char texto[25]; strftime(texto, sizeof(texto), "%Y-%m-%dT%H:%M:%SZ", &utc);
  return String(texto);
}
void guardarRegistro(uint32_t ahora) {
  if (!almacenamientoOK || !tieneMuestra) return;
  int marca = pruebaActiva ? -3 : (fase != MONITOREO ? -2 : (falloSenal ? -1 : int(estado)));
  if (marca == ultimaMarcaRegistro && ahora - ultimoRegistroMs < PERIODO_REGISTRO_MS) return;
  ultimoRegistroMs = ahora; ultimaMarcaRegistro = marca;
  if (!crearRegistro()) { errorRegistro = true; return; }
  File existente = LittleFS.open(ARCHIVO, "r");
  bool lleno = existente && existente.size() >= limiteArchivo;
  existente.close();
  if (lleno && !rotarRegistro()) { errorRegistro = true; return; }
  bool valida = referenciaLista && senalValida && !falloSenal;
  String linea = String(sesion) + "," + String(ahora) + "," + fechaUTC() + ",";
  linea += String(voltajeAO(), 3) + "," + String(lecturaFiltrada, 1) + ",";
  if (referenciaLista) linea += String(referencia, 1);
  linea += ",";
  if (valida) linea += String(aumentoADC(), 1);
  linea += ",";
  if (valida) linea += String(porcentajeADC(), 2);
  linea += "," + String(nombreFase()) + "," + String(valida && !pruebaActiva ? int(estado) : -1);
  linea += pruebaActiva ? ",1\n" : ",0\n";
  File f = LittleFS.open(ARCHIVO, "a");
  if (!f) { errorRegistro = true; return; }
  errorRegistro = f.print(linea) != linea.length();
  f.close();  // Cerrar cada registro limita la perdida ante un corte.
}

void jsonNumero(String& json, float valor, uint8_t decimales = 1) {
  json += String(valor, (unsigned int)decimales);
}
void enviarDatos() {
  uint32_t ahora = millis();
  ResumenNube n = copiarNube();
  bool valida = referenciaLista && senalValida && !falloSenal;
  String j; j.reserve(1600);
  j = "{\"sesion\":\"" + String(sesion) + "\",\"tiempo_ms\":" + String(ahora);
  j += ",\"secuencia\":" + String(secuencia) + ",\"fase\":\"" + String(nombreFase()) + "\"";
  j += ",\"listo\":" + String(referenciaLista ? "true" : "false");
  j += ",\"senal_valida\":" + String(senalValida && !falloSenal ? "true" : "false");
  j += ",\"restante_s\":" + String(fase == INICIO && ahora - inicioMs < ESPERA_INICIAL_MS ? (ESPERA_INICIAL_MS - (ahora - inicioMs) + 999) / 1000 : 0);
  j += ",\"referencia_muestras\":" + String(muestrasReferencia);
  j += ",\"adc\":"; if (tieneMuestra) jsonNumero(j, lecturaFiltrada); else j += "null";
  j += ",\"voltaje_pin_v\":"; if (tieneMuestra) jsonNumero(j, mvFiltrados / 1000, 3); else j += "null";
  j += ",\"voltaje_ao_v\":"; if (tieneMuestra) jsonNumero(j, voltajeAO(), 3); else j += "null";
  j += ",\"referencia_adc\":"; if (referenciaLista) jsonNumero(j, referencia); else j += "null";
  j += ",\"aumento_adc\":"; if (valida) jsonNumero(j, aumentoADC()); else j += "null";
  j += ",\"cambio_pct\":"; if (valida) jsonNumero(j, porcentajeADC(), 2); else j += "null";
  j += ",\"estado\":" + String(valida ? int(estado) : -1);
  j += ",\"alertas\":" + String(alertas);
  const char* calidad = !tieneMuestra ? "Sin muestra" : (!senalValida || falloSenal ? "Revisar AO / alimentacion" : (mvFiltrados < 150 ? "Voltaje bajo: menor precision ADC" : "Dentro del margen electrico observado"));
  j += ",\"calidad\":\"" + String(calidad) + "\"";
  j += ",\"prueba\":" + String(pruebaActiva ? "true" : "false");
  j += ",\"salidas\":{\"verde\":" + String(salidaVerde ? "true" : "false");
  j += ",\"amarillo\":" + String(salidaAmarillo ? "true" : "false");
  j += ",\"rojo\":" + String(salidaRojo ? "true" : "false");
  j += ",\"buzzer\":" + String(buzzerEncendido ? "true" : "false") + "}";
  j += ",\"wifi_conectado\":" + String(WiFi.status() == WL_CONNECTED ? "true" : "false");
  j += ",\"ip_sta\":\"" + WiFi.localIP().toString() + "\"";
  j += ",\"nube\":{\"estado\":\"" + String(textoNube(n.resultado)) + "\",\"http\":" + String(n.http);
  j += ",\"envios\":" + String(n.envios) + ",\"entrada\":" + String(n.entrada);
  j += ",\"edad_ms\":" + String(n.envios ? ahora - n.ultimoExitoMs : 0) + "}";
  j += ",\"almacenamiento\":{\"disponible\":" + String(almacenamientoOK ? "true" : "false");
  j += ",\"error\":" + String(errorRegistro ? "true" : "false");
  j += ",\"usado\":" + String(almacenamientoOK ? (uint32_t)LittleFS.usedBytes() : 0);
  j += ",\"total\":" + String(almacenamientoOK ? (uint32_t)LittleFS.totalBytes() : 0) + "}}";
  servidor.sendHeader("Cache-Control", "no-store");
  servidor.send(200, "application/json; charset=utf-8", j);
}

void respuesta(int codigo, const char* mensaje) {
  servidor.send(codigo, "application/json; charset=utf-8", "{\"mensaje\":\"" + String(mensaje) + "\"}");
}
void descargarCSV(const char* ruta, const char* nombre) {
  if (!almacenamientoOK) { respuesta(503, "Registro sin inicializar."); return; }
  File f = LittleFS.open(ruta, "r");
  if (!f) { respuesta(404, "Todavia no hay archivo para descargar."); return; }
  servidor.sendHeader("Content-Disposition", "attachment; filename=" + String(nombre));
  servidor.sendHeader("Cache-Control", "no-store");
  servidor.streamFile(f, "text/csv; charset=utf-8"); f.close();
}
void configurarWeb() {
  servidor.on("/", HTTP_GET, []() { servidor.send_P(200, "text/html; charset=utf-8", DASHBOARD); });
  servidor.on("/datos", HTTP_GET, enviarDatos);
  servidor.on("/registro.csv", HTTP_GET, []() { descargarCSV(ARCHIVO, "CAMI_actual.csv"); });
  servidor.on("/anterior.csv", HTTP_GET, []() { descargarCSV(ANTERIOR, "CAMI_anterior.csv"); });
  servidor.on("/prueba", HTTP_POST, []() {
    if (!referenciaLista || !senalValida || falloSenal || estado == ALERTA || pruebaActiva) {
      respuesta(409, "Espera una referencia valida y un estado sin alerta roja para probar."); return;
    }
    pruebaActiva = true; inicioPruebaMs = millis();
    respuesta(200, "Prueba iniciada: verde, amarillo, rojo con buzzer; un segundo por etapa.");
  });
  servidor.on("/referencia", HTTP_POST, []() {
    if (!referenciaLista || !senalValida || falloSenal || estado != NORMAL || pruebaActiva) {
      respuesta(409, "No se cambia la referencia durante una alerta, falla o prueba."); return;
    }
    fase = REFERENCIA; referenciaLista = false; sumaReferencia = 0; muestrasReferencia = 0;
    respuesta(200, "Tomando nueva referencia durante unos 5 segundos. No es calibracion de CO.");
  });
  servidor.on("/inicializar", HTTP_POST, []() {
    if (almacenamientoOK) { respuesta(409, "El registro ya esta inicializado; no se borro."); return; }
    // Nunca formatear automaticamente despues de una falla de montaje.
    // Esta accion requiere el boton y confirmacion explicita del dashboard.
    LittleFS.end();
    if (!LittleFS.format() || !LittleFS.begin(false)) {
      respuesta(500, "No se pudo inicializar: revisa Partition Scheme con SPIFFS."); return;
    }
    almacenamientoOK = true; actualizarLimite(); errorRegistro = !crearRegistro();
    respuesta(200, "Registro inicializado. Los nuevos datos se guardaran en memoria.");
  });
  servidor.onNotFound([]() { respuesta(404, "Ruta no encontrada."); });
  servidor.begin();
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_SENSOR, INPUT); pinMode(PIN_BUZZER, OUTPUT);
  pinMode(LED_VERDE, OUTPUT); pinMode(LED_AMARILLO, OUTPUT); pinMode(LED_ROJO, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW); escribirSalidas(false, false, false, false);
  analogReadResolution(12); analogSetPinAttenuation(PIN_SENSOR, ADC_11db);
  snprintf(sesion, sizeof(sesion), "%08lx", (unsigned long)esp_random());
  almacenamientoOK = LittleFS.begin(false);
  if (almacenamientoOK) { actualizarLimite(); errorRegistro = !crearRegistro(); }
  else Serial.println("Registro sin montar. Pulsa Inicializar registro en el dashboard la primera vez.");
  WiFi.mode(WIFI_AP_STA);
  WiFi.setAutoReconnect(true);
  WiFi.softAPConfig(IPAddress(192,168,4,1), IPAddress(192,168,4,1), IPAddress(255,255,255,0));
  bool apOK = WiFi.softAP(AP_NOMBRE, AP_CLAVE);
  if (!apOK) Serial.println("ERROR: no se pudo iniciar el punto de acceso CAMI.");
  if (wifiConfigurado()) WiFi.begin(WIFI_SSID, WIFI_CLAVE);
  configurarWeb();
  colaNube = xQueueCreate(1, sizeof(MuestraNube));
  if (!colaNube || xTaskCreate(tareaThingSpeak, "ThingSpeak", 12288, nullptr, 1, nullptr) != pdPASS) {
    if (colaNube) { vQueueDelete(colaNube); colaNube = nullptr; }
    establecerNube(5);
  }
  inicioMs = millis(); ultimaMuestraMs = inicioMs - PERIODO_MUESTRA_MS;
  Serial.println("CAMI: espera inicial de senal de 60 segundos; despues referencia de 5 segundos.");
  Serial.println("Wi-Fi local: CAMI | clave: CAMI12345 | http://192.168.4.1");
  Serial.println("MQ7 AO=34 | buzzer=23 | verde=25 | amarillo=26 | rojo=27");
}

void loop() {
  uint32_t ahora = millis();
  procesarSensor(ahora);
  actualizarSalidas(ahora);
  if (ahora - ultimoHistorialMs >= PERIODO_HISTORIAL_MS) {
    ultimoHistorialMs = ahora; ++secuencia;
  }
  guardarRegistro(ahora);
  atenderRed(ahora);
  servidor.handleClient();
  if (ahora - ultimoSerialMs >= 1000) {
    ultimoSerialMs = ahora;
    Serial.printf("ADC=%.1f | AO estimado=%.3f V | Ref=%.1f | Aumento=%.1f | %s%s\n",
      lecturaFiltrada, voltajeAO(), referencia, referenciaLista ? aumentoADC() : 0,
      nombreFase(), pruebaActiva ? " | PRUEBA MANUAL" : "");
  }
  delay(1);
}
