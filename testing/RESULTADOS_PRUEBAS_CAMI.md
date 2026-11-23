# CAMI — Resultados de pruebas y evidencias audiovisuales

**Proyecto:** CAMI — Casco inteligente de monitoreo ambiental  
**Asignatura:** TEI201 — Taller de Introducción a la Ingeniería  
**Registro documental:** 23 de noviembre de 2026  
**Estado:** Evidencia de luces, casco y dashboard local incorporada. Algunas verificaciones del protocolo siguen pendientes.

## 1. Alcance y origen de las evidencias

Este documento presenta **lo observable en dos videos proporcionados por el equipo**. La fecha y hora exactas de grabación no se han verificado. Los videos documentan el uso de un casco con módulos y una prueba de mesa de las salidas luminosas. **Los dos videos descritos en esta tabla no muestran el dashboard; para ello se incorporan las dos grabaciones adicionales del 23 de noviembre y las capturas documentadas en [`DASHBOARD_EVIDENCIA_CAMI.md`](DASHBOARD_EVIDENCIA_CAMI.md).**

| Evidencia | Duración aproximada del archivo | Contenido observable | Archivo propuesto |
|---|---:|---|---|
| V1 — Casco puesto | 12,7 s | Integrante utilizando casco blanco con una caja o módulo montado en el lateral; interacción con el prototipo durante la grabación. | [CAMI_01_Casco_puesto.mov](evidencias/CAMI_01_Casco_puesto.mov) |
| V2 — Prueba electrónica de mesa | 21,9 s | Módulo de sensor y componentes electrónicos, cables, carcasas y semáforo de tres colores; se observan **en distintos momentos** el LED verde y el LED rojo encendidos. | [CAMI_02_Prueba_electronica.mov](evidencias/CAMI_02_Prueba_electronica.mov) |

**Fotogramas de referencia:**

- [Casco con módulo lateral](evidencias/CAMI_casco_fotograma.jpg).
- [Circuito con LED verde encendido](evidencias/CAMI_led_verde.jpg).
- [Circuito con LED rojo encendido](evidencias/CAMI_led_rojo.jpg).

Las duraciones son las de los archivos audiovisuales, **no la duración total de los ensayos** ni una medición del tiempo de respuesta del sistema.

## 2. Resultados frente al protocolo

El procedimiento previsto está descrito en [`Documentación Técnica/PROTOCOLO_PRUEBAS_CAMI.md`](../Documentaci%C3%B3n%20T%C3%A9cnica/PROTOCOLO_PRUEBAS_CAMI.md).

| Caso | Resultado sustentado por los videos | Estado de verificación | Evidencia |
|---|---|---|---|
| P1 — Encendido y lectura continua | En la grabación de pantalla se observan lecturas ADC variables (por ejemplo 0, 261 y 1540), con gráfico de tendencia. | **Parcial:** hay lecturas observables, sin prueba de estabilidad prolongada. | Grabación del dashboard |
| P2 — Dashboard local | Se observa el panel en `192.168.4.1` y la transición verde → amarillo → rojo, con voltaje, ADC y tendencia. | **Verificado en la grabación** para una sesión de demostración. | [`DASHBOARD_EVIDENCIA_CAMI.md`](DASHBOARD_EVIDENCIA_CAMI.md) |
| P3 — Registro persistente | No hay CSV ni registro visible que permita comprobar almacenamiento tras reiniciar. | **Pendiente**. | — |
| P4 — ThingSpeak | No se visualiza el canal con datos atribuibles a esta prueba. | **Pendiente**. | — |
| P5 — Semáforo y buzzer | En V2 se observan LED verde y rojo encendidos en momentos distintos. La captura del dashboard muestra estados verde, amarillo y rojo; durante el rojo indica `Buzzer ON`. No se confirma acústicamente el buzzer solo por la captura. | **Parcial:** se observan luces y el indicador lógico del buzzer. | V2 y dashboard |
| P6 — Encapsulado | V1 muestra el casco puesto con módulo lateral; V2 muestra carcasas y componentes separados durante la prueba. | **Parcial:** falta registro de tres ángulos y apertura/cierre sin daño. | V1, V2 |

## 3. Observaciones verificables de la prueba de mesa

1. **Montaje electrónico:** se aprecia un sensor con carcasa metálica perforada, una placa electrónica, cableado y un módulo con tres indicadores de colores.
2. **Indicador verde:** se observa iluminado en parte del video V2, por ejemplo, cerca del segundo 12.
3. **Indicador rojo:** se observa iluminado posteriormente, por ejemplo, cerca del segundo 18.
4. **Alcance:** la grabación documenta una **secuencia visual de salidas**. Por sí sola no demuestra medición cuantitativa de CO, precisión, detección certificada ni sincronización con el panel web.

### Datos cuantitativos y aspectos aún no acreditados

| Indicador | Valor registrado |
|---|---|
| Concentración de CO (ppm) | **No medida ni calibrada.** |
| ADC filtrado durante la prueba del dashboard | Se observan 0 ADC en verde, 261 ADC en amarillo y 1540 ADC en rojo (capturas, valores puntuales). |
| Estado y umbral de pantalla | Los colores cambiaron al variar la señal; el dashboard muestra umbrales experimentales de aumento ADC, no ppm. |
| Tiempo de respuesta al estímulo | No medido. |
| Registros guardados en LittleFS / CSV | El dashboard exhibe registros/uso de memoria; no hay demostración de persistencia tras reinicio ni archivo CSV adjunto. |
| Nuevas entradas reales en ThingSpeak | Sin evidencia adjunta. |
| Alarma sonora | El dashboard indica `Buzzer ON` durante rojo; falta corroboración audible independiente. |

## 4. Fallas, limitaciones y mejoras

**Falla real corregida:** todavía no hay registro suficiente para describir una falla específica, su causa y la corrección efectivamente implementada. Debe agregarse un caso real documentado por el equipo, tal como exige la pauta; no se inventará uno.

**Limitaciones evidenciadas:** los videos de mesa por sí solos no verifican la correspondencia cuantitativa entre lecturas ADC y las luces; la grabación adicional sí demuestra las transiciones del dashboard local. Sigue pendiente demostrar almacenamiento persistente. Para reforzar el registro, se recomienda grabar en una misma toma el circuito y el dashboard, o conservar capturas de ambos con hora consistente; también documentar la prueba de buzzer y cualquier falla real resuelta.

## 5. Relación con el problema original

CAMI fue propuesto para explorar un sistema de advertencia de condiciones ambientales potencialmente peligrosas en minería subterránea. Las evidencias disponibles apoyan que el equipo **construyó un montaje de casco y pudo mostrar al menos dos estados luminosos** en el circuito de pruebas. Es una validación parcial de montaje y señalización, no una validación de detección fiable de gases ni de aptitud de uso minero.

El sensor MQ-7 utilizado **no está calibrado aquí para cuantificar CO en ppm** y el firmware del repositorio no implementa el ciclo térmico requerido para una medición validada. Por ello, CAMI es un **prototipo académico** que no debe utilizarse como equipo de seguridad personal.

## 6. Prueba del dashboard — 23 de noviembre de 2026

La grabación de pantalla `ScreenRecording_11-23-2026 16-15-05_1.mp4` y el video externo `IMG_5212.mp4` registran la prueba realizada por el equipo. El dashboard local muestra:

| Estado | Voltaje AO estimado | Señal filtrada | Entradas en rojo | Indicador de buzzer |
|---|---:|---:|---:|---|
| Verde — Señal bajo el umbral | 0.284 V | 0 ADC | 0 | OFF |
| Amarillo — Aumento de señal | 0.697 V | 261 ADC | 0 | OFF |
| Rojo — Alerta por aumento | 2.719 V | 1540 ADC | 1 | ON |

Se observa también un gráfico en que la señal sube y después desciende. El video externo permite ver simultáneamente el casco y la tablet con la interfaz. Esto **respalda el funcionamiento del dashboard local y su respuesta a variaciones de señal**, sin acreditar lectura de CO en ppm ni transmisión exitosa a ThingSpeak. Más detalles y seis fotogramas en [`DASHBOARD_EVIDENCIA_CAMI.md`](DASHBOARD_EVIDENCIA_CAMI.md).

## 7. Pendientes para cerrar Testing

- [x] Videos de casco y de funcionamiento visual del semáforo disponibles.
- [x] Observaciones reales de los videos documentadas, distinguiendo lo visible de lo no demostrado.
- [x] Capturas del dashboard local mostrando lecturas, gráfico y estados de la demostración.
- [ ] Archivo CSV descargado o prueba de persistencia y, si aplica, confirmación de envíos reales a ThingSpeak (no acreditados en los videos).
- [ ] Confirmación del buzzer y condición bajo la que cambian las luces.
- [ ] Fotos del casco completo de frente, lateral y atrás (solo si realmente están disponibles).
- [ ] Una falla realmente encontrada, su corrección y resultado de la repetición.

**Nota de privacidad:** antes de publicar videos o imágenes que muestran personas reconocibles en un repositorio público, confirmar que las personas grabadas aceptan su publicación. Si no, usar fotogramas del hardware sin rostros.
