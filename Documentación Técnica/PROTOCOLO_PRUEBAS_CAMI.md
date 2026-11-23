# CAMI — Protocolo de pruebas y validación

**Asignatura:** Taller de Introducción a la Ingeniería (TEI201)  
**Proyecto:** CAMI — Casco inteligente de monitoreo ambiental  
**Versión:** 1.0 — 23 de noviembre de 2026  
**Estado:** Protocolo preparado; resultados y evidencias pendientes de completar por el equipo.

## 1. Objetivo y alcance

Verificar con el prototipo físico que la ESP32 recibe lecturas reales del sensor MQ-7, que el sistema muestra la señal en el panel web, que registra información de forma persistente, que comunica datos a ThingSpeak si está configurado y que las alertas luminosas/sonoras responden a los estados del firmware.

**Limitación esencial:** El MQ-7 no está calibrado para medir concentraciones de CO en ppm y el firmware no implementa el ciclo térmico requerido para una medición validada de CO. Los valores ADC y los umbrales experimentales **no prueban que el aire sea seguro**. CAMI no es un detector certificado ni un equipo de protección personal.

## 2. Condiciones y seguridad

- Utilizar el casco y el circuito en un entorno ventilado, seco y supervisado.
- Comprobar visualmente cables, polaridad y alimentación antes de encender. **No conectar dos celdas 18650 directamente al pin de 5 V sin regulación adecuada.** Si hay dudas sobre la fuente, consultar al docente y usar una alimentación USB apropiada para la ESP32.
- No generar CO, humo, combustión, gases tóxicos ni exposición intencional a sustancias para probar el sensor.
- Usar el teléfono/iPad conectado a la red local de la ESP32 para el panel `http://192.168.4.1`; anotar si el equipo realmente lo abre.
- Registrar fecha, responsables, configuración y duración de cada prueba.
- Tomar fotos o capturas de **datos reales**. Distinguir claramente las capturas simuladas de las mediciones físicas.

**Fecha de ejecución:** [pendiente]  
**Responsables:** [pendiente]  
**Alimentación utilizada:** [pendiente]  
**Versión de firmware:** [pendiente]  
**Duración total:** [pendiente]

## 3. Casos de prueba

### P1 — Encendido y lectura continua

- **Objetivo:** Comprobar que la ESP32 inicia y recibe lecturas analógicas reales del MQ-7.
- **Condiciones:** Circuito ensamblado y alimentado correctamente; ambiente ventilado.
- **Procedimiento:** (1) Encender el prototipo; (2) esperar la inicialización indicada por el firmware; (3) abrir el panel local; (4) observar las lecturas durante 5 minutos; (5) anotar el número de lecturas y las interrupciones visibles.
- **Criterio de éxito:** Lecturas actualizadas durante la observación, sin bloqueo del sistema ni pérdida prolongada de datos.
- **Evidencia:** Captura del panel con hora y, si es posible, registro de datos.
- **Resultado real:** [pendiente]

### P2 — Dashboard local

- **Objetivo:** Verificar que el usuario puede interpretar la lectura y el estado de las alertas.
- **Procedimiento:** (1) Conectar iPad o teléfono a la red local del prototipo; (2) abrir `http://192.168.4.1`; (3) verificar lectura ADC, estado de indicadores y actualización; (4) capturar pantalla.
- **Criterio de éxito:** Panel accesible y valores/estados actualizados desde el dispositivo físico.
- **Evidencia:** Captura real del dashboard y foto del prototipo encendido.
- **Resultado real:** [pendiente]

### P3 — Registro persistente

- **Objetivo:** Confirmar que las lecturas quedan guardadas y pueden consultarse después.
- **Procedimiento:** (1) Encender el dispositivo y registrar datos por al menos 5 minutos; (2) consultar o descargar el CSV almacenado en LittleFS según las funciones disponibles del firmware; (3) verificar marcas temporales y valores; (4) reiniciar y comprobar si el histórico se conserva.
- **Criterio de éxito:** Registros reales estructurados, consultables después de reiniciar.
- **Evidencia:** CSV o captura de registros antes y después del reinicio.
- **Resultado real:** [pendiente]

### P4 — Envío de datos a ThingSpeak

- **Objetivo:** Comprobar que la información del prototipo llega al canal remoto.
- **Procedimiento:** (1) Confirmar que la ESP32 tiene conectividad de red configurada; (2) encender y esperar los intervalos de envío; (3) abrir `https://thingspeak.mathworks.com/channels/3571982`; (4) contrastar fecha y hora de nuevas entradas con la sesión real; (5) registrar cantidad de entradas nuevas y posibles fallas.
- **Criterio de éxito:** Nuevos datos reales del prototipo aparecen en el canal durante la prueba, con tiempos coherentes.
- **Evidencia:** Capturas de la gráfica con fecha/hora y registros del dispositivo, si están disponibles.
- **Resultado real:** [pendiente]

### P5 — Semáforo y buzzer

- **Objetivo:** Verificar las salidas físicas de señalización y su correspondencia con el estado del programa.
- **Procedimiento:** (1) Registrar el estado de las luces y buzzer durante encendido y operación normal; (2) si el firmware incluye un modo de prueba de salidas seguro, ejecutarlo y dejar constancia de que es una prueba funcional, no una detección de gas; (3) comprobar qué luces y sonido efectivamente se activan; (4) fotografiar o filmar la secuencia.
- **Criterio de éxito:** Salidas físicas funcionan y coinciden con las órdenes del programa. **No inducir una alarma con gases o combustión.**
- **Evidencia:** Fotos/video del semáforo y buzzer, y registro del estado de firmware.
- **Resultado real:** [pendiente]

### P6 — Encapsulado y mantenimiento

- **Objetivo:** Comprobar que el hardware está sujeto al casco y puede inspeccionarse sin dañar el sistema.
- **Procedimiento:** (1) Fotografiar el casco armado desde frente, costado y parte posterior; (2) comprobar sujeción de las tres cajas; (3) abrir una carcasa siguiendo el diseño; (4) inspeccionar el acceso a componentes y cerrar nuevamente.
- **Criterio de éxito:** Componentes accesibles, sin cables forzados ni piezas rotas durante la apertura/cierre.
- **Evidencia:** Mínimo tres fotografías del casco ensamblado y fotos del interior.
- **Resultado real:** [pendiente]

## 4. Registro cuantitativo

| Prueba | Fecha/hora | Duración | Lecturas o registros observados | Interrupciones/errores | Resultado (cumple/no cumple) | Archivo de evidencia |
|---|---|---|---|---|---|---|
| P1 | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente |
| P2 | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente |
| P3 | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente |
| P4 | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente |
| P5 | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente |
| P6 | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente | Pendiente |

Cuando existan datos suficientes, calcular, según corresponda: **tasa de recepción** = registros recibidos / registros esperados × 100 %, **latencia** = hora de visualización − hora de captura y **tasa de fallos** = intentos fallidos / intentos totales × 100 %. No informar precisión de detección de CO sin instrumento de referencia y calibración.

## 5. Análisis de fallas y mejoras

La rúbrica solicita identificar y analizar al menos una falla real y explicar su solución. **No inventar fallas ni soluciones.** Completar después de probar:

- **Falla observada:** [pendiente]
- **Cuándo ocurrió y cómo se detectó:** [pendiente]
- **Causa probable y evidencia:** [pendiente]
- **Acción correctiva realizada:** [pendiente]
- **Resultado de repetir la prueba:** [pendiente]
- **Mejora futura fundamentada:** [pendiente]

## 6. Relación con el problema original

El problema abordado por CAMI es la falta de advertencia temprana frente a posibles condiciones atmosféricas peligrosas para trabajadores de minería subterránea. Estas pruebas deben demostrar, a nivel de **prototipo académico**, que el sistema puede captar una señal ambiental, comunicarla al usuario y dejar un historial para analizar tendencias.

**Alcance de la validación:** Verificar comunicación, registro y alertas no equivale a validar detección fiable de CO ni a demostrar seguridad en una mina. Para ello serían necesarios un sensor y un sistema adecuados, calibración con patrones, pruebas controladas y evaluación de requisitos de seguridad aplicables.

**Conclusión basada en resultados reales:** [pendiente de completar]

## 7. Archivos de evidencia por adjuntar

- Fotografías del casco ensamblado desde al menos tres ángulos.
- Capturas reales del dashboard local.
- Gráficas reales de ThingSpeak con fecha/hora de la prueba, si se valida la transmisión.
- Archivo CSV o registros reales de mediciones, si se valida el almacenamiento.
- Fotografías de apertura del encapsulado y, cuando sea posible, del montaje interno.

**Nota:** Este documento es el **protocolo**, no un informe de pruebas ya realizadas. Completar los campos pendientes después de ejecutar los ensayos y subir la evidencia al repositorio.
