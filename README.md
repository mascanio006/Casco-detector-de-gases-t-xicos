# CAMI — Casco inteligente de monitoreo ambiental

**Asignatura:** Taller de Introducción a la Ingeniería (TEI201)  
**Proyecto:** CAMI — prototipo IoT de monitoreo de variaciones en la señal de un sensor de gases.

CAMI es un prototipo académico de casco que incorpora una **ESP32 DevKit V1** y un **sensor MQ-7**. Su finalidad es explorar una forma de advertir cambios en la señal del sensor mediante luces, buzzer y un dashboard web local, además de permitir el registro de datos para su análisis. La propuesta se relaciona con el problema de la exposición a atmósferas peligrosas en minería subterránea.

> **Advertencia:** CAMI **no es un detector de CO certificado**, no mide concentraciones calibradas en ppm y no permite confirmar que un ambiente sea seguro. Las alertas representan umbrales experimentales de señal ADC; el firmware no implementa el ciclo térmico del MQ-7 necesario para mediciones validadas de CO. No usar como equipo de protección personal.

## Integrantes del equipo

- Mariana Ascanio
- Antonia Briceño
- Emilia Echavarría
- Ainara Felix
- Andrés Obreque

## ¿Cómo funciona?

1. El sensor MQ-7 entrega una señal analógica al sistema.
2. La ESP32 lee esa señal por **GPIO34**, a través de un divisor resistivo indicado en la documentación técnica.
3. El programa calcula una referencia experimental y sigue los cambios de la señal filtrada.
4. De acuerdo con el aumento respecto de esa referencia, el firmware ordena estados **normal**, **prealerta** y **alerta**.
5. El dashboard local muestra las lecturas, el gráfico de tendencia, los estados ordenados por el programa y el estado de conectividad y almacenamiento.
6. El firmware incluye registro CSV en LittleFS y envío opcional a ThingSpeak, cuya operación real debe validarse con evidencias del prototipo.

### Componentes principales

| Componente | Función |
|---|---|
| ESP32 DevKit V1 | Procesamiento de lecturas, control y servidor web local |
| Sensor MQ-7 | Señal analógica experimental |
| Módulo de tres luces (verde, amarilla y roja) | Indicadores visuales de estado |
| Buzzer y transistor BC547 | Alarma sonora controlada por la ESP32 |
| Resistencias y cableado | Adaptación de lectura y conexiones |
| Baterías 18650, portabaterías e interruptor | Alimentación prevista; verificar regulación y protección antes de su uso |
| Tres cajas y tres tapas impresas en 3D | Encapsulado modular para control, alimentación y alertas |

El listado de materiales, cantidades y gastos registrados está en [BOM_CAMI.md](<Documentación Técnica/BOM_CAMI.md>). **Gasto registrado al 23 de noviembre de 2026: $41.170 CLP**, con partidas aún por verificar.

### Conexiones programadas

| Elemento | Pin ESP32 |
|---|---|
| Señal analógica MQ-7 (AO), por divisor 10 kΩ / 10 kΩ | GPIO34 |
| LED verde | GPIO25 |
| LED amarillo | GPIO26 |
| LED rojo | GPIO27 |
| Buzzer, mediante BC547 | GPIO23 |

Estas conexiones se documentan según el firmware y **deben contrastarse con el montaje físico**. Ver [diagrama de conexiones](<Documentación Técnica/Diagramas de Circuito/DIAGRAMA_CONEXIONES_CAMI.md>). El archivo `Diagrama.jpeg` corresponde a una versión previa con MQ-135 y no reemplaza el esquemático final.

## Instalación y uso del software

### 1. Preparar el entorno

- Instalar **Arduino IDE** y el soporte de placas **ESP32 de Espressif Systems**.
- Conectar la ESP32 al computador mediante un cable USB que permita transferencia de datos.
- Tener disponible el prototipo montado y una alimentación adecuada, verificada previamente por el equipo. **No conectar directamente dos baterías 18650 al pin de 5 V de la ESP32 sin regulación comprobada.**

### 2. Abrir y cargar el firmware

1. Clonar o descargar este repositorio desde **Code → Download ZIP** en GitHub.
2. Abrir [`CAMI_FINAL_GITHUB.ino`](CAMI_FINAL_GITHUB.ino) en Arduino IDE. Si el IDE solicita crear una carpeta de sketch con el nombre del archivo, aceptar.
3. Seleccionar la placa compatible (habitualmente **ESP32 Dev Module** para la ESP32 DevKit V1) y el puerto correspondiente.
4. Revisar las conexiones físicas antes de alimentar el prototipo y compilar/subir el programa.

El firmware utiliza bibliotecas incluidas con el núcleo Arduino para ESP32. No declara bibliotecas externas adicionales.

**Credenciales:** el repositorio contiene valores de ejemplo (`TU_WIFI`, `TU_CLAVE_WIFI`, `REEMPLAZAR_EN_COPIA_LOCAL`). Si se habilita internet/ThingSpeak, reemplazarlos **solo en una copia local**, sin publicar contraseñas ni claves de escritura reales en GitHub.

### 3. Abrir el dashboard local

1. Encender la ESP32 con el firmware cargado.
2. Conectar el teléfono, tablet o computador a la red Wi-Fi **`CAMI`** que crea la placa, con clave de demostración **`CAMI12345`**.
3. Abrir en el navegador: **http://192.168.4.1**.
4. Esperar la secuencia de inicio programada: **60 segundos** iniciales y aproximadamente **5 segundos** para obtener la referencia experimental. Esta espera **no equivale a calibración de gases**.
5. Observar voltaje AO estimado, ADC filtrado, aumento respecto de referencia, gráfico, estado, alertas ordenadas y conectividad.

El panel incorpora la opción **“Probar luces y buzzer”**, que corresponde a una prueba de actuadores y **no simula una medición real de gas**. También permite descargar los archivos CSV local y anterior cuando el almacenamiento esté disponible.

### 4. ThingSpeak (opcional)

El código contempla el canal [ThingSpeak 3571982](https://thingspeak.mathworks.com/channels/3571982), con tres campos previstos:

| Campo | Dato previsto |
|---|---|
| 1 | Voltaje AO estimado |
| 2 | ADC filtrado |
| 3 | Estado (0 = normal, 1 = prealerta, 2 = alerta) |

Para el envío se necesita Wi-Fi de **2,4 GHz** con acceso a internet, las credenciales configuradas localmente y la **clave de escritura del canal**. El intervalo de envío programado es de **20 segundos** bajo condiciones válidas. Las capturas que ya están en `docs/` corresponden a datos simulados; **no demuestran que la ESP32 haya enviado mediciones reales**.

## Organización de los archivos disponibles

| Archivo o carpeta | Contenido |
|---|---|
| [`CAMI_FINAL_GITHUB.ino`](CAMI_FINAL_GITHUB.ino) | Firmware y dashboard web integrado |
| [`FUENTES.md`](FUENTES.md) | Fuentes, bibliotecas y uso de IA |
| [`DOCUMENTACION_ACTUALIZADA.md`](DOCUMENTACION_ACTUALIZADA.md) | Evolución física, tres carcasas y aspectos por verificar |
| [`Documentación Técnica/BOM_CAMI.md`](<Documentación Técnica/BOM_CAMI.md>) | Lista de materiales y gastos |
| [`Documentación Técnica/PROTOCOLO_PRUEBAS_CAMI.md`](<Documentación Técnica/PROTOCOLO_PRUEBAS_CAMI.md>) | Protocolo de pruebas; resultados aún por completar |
| [`Documentación Técnica/Diagramas de Circuito/`](<Documentación Técnica/Diagramas de Circuito/>) | Cableado documentado y diagrama de una versión anterior |
| [`Documentación Técnica/Planos Acotados/`](<Documentación Técnica/Planos Acotados/>) | Seis planos en PDF |
| [`Conceptualización del Encapsulado/Modelo 3D/`](<Conceptualización del Encapsulado/Modelo 3D/>) | Renders del diseño |
| [`docs/`](docs/) | Capturas de ThingSpeak, identificadas como simuladas |
| Archivos `.f3d` de la raíz | Modelos editables de las tres cajas y sus tres tapas |
| Fotografías `CAJA_1_CONTROL.jpg`, `CAJA_2_BATERIAS.jpg`, `CAJA_3_ALERTAS.jpg` | Registro fotográfico de los módulos individuales |

## Validación y próximos pasos

El [protocolo de pruebas](<Documentación Técnica/PROTOCOLO_PRUEBAS_CAMI.md>) describe pruebas de lectura, dashboard, registro CSV, conectividad, alertas y encapsulado. **Es necesario completar los resultados con mediciones, observaciones y evidencias reales del equipo.**

Antes del cierre definitivo del repositorio, corresponde:

- Incorporar un **esquemático eléctrico final** comprobado contra el casco ensamblado (no solo el diagrama de cableado).
- Añadir **fotografías del casco completo desde al menos tres ángulos** y resultados de las pruebas realmente ejecutadas.
- Verificar la fuente de alimentación, el interruptor, las protecciones, el buzzer y las conexiones reales.
- Documentar **al menos una falla real y la corrección efectuada**, si corresponde al trabajo realizado.
- Organizar finalmente las entregas en las carpetas requeridas por la pauta (`firmware/`, `hardware/`, `diseño-3d/`, `testing/`, `docs/`), conservando los archivos existentes.

## Fuentes e integridad académica

Las fuentes técnicas y la declaración de apoyo de herramientas de IA están documentadas en [`FUENTES.md`](FUENTES.md). El material de este repositorio constituye documentación de un **prototipo académico**, no resultados de una certificación técnica.
