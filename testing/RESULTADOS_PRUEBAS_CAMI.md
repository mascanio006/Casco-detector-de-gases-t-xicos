# CAMI — Resultados de pruebas y evidencias audiovisuales

**Proyecto:** CAMI — Casco inteligente de monitoreo ambiental  
**Asignatura:** TEI201 — Taller de Introducción a la Ingeniería  
**Registro documental:** 23 de noviembre de 2026  
**Estado:** Evidencia visual incorporada. Algunos casos del protocolo aún requieren verificación instrumental o registros.

## 1. Alcance y origen de las evidencias

Este documento presenta **lo observable en dos videos proporcionados por el equipo**. La fecha y hora exactas de grabación no se han verificado. Los videos documentan el uso de un casco con módulos y una prueba de mesa de las salidas luminosas. **No muestran la pantalla del dashboard ni valores numéricos de medición.**

| Evidencia | Duración aproximada del archivo | Contenido observable | Archivo propuesto |
|---|---:|---|---|
| V1 — Casco puesto | 12,7 s | Integrante utilizando casco blanco con una caja o módulo montado en el lateral; interacción con el prototipo durante la grabación. | [CAMI_01_Casco_puesto.mp4](evidencias/CAMI_01_Casco_puesto.mp4) |
| V2 — Prueba electrónica de mesa | 21,9 s | Módulo de sensor y componentes electrónicos, cables, carcasas y semáforo de tres colores; se observan **en distintos momentos** el LED verde y el LED rojo encendidos. | [CAMI_02_Prueba_electronica.mp4](evidencias/CAMI_02_Prueba_electronica.mp4) |

**Fotogramas de referencia:**

- [Casco con módulo lateral](evidencias/CAMI_casco_fotograma.jpg).
- [Circuito con LED verde encendido](evidencias/CAMI_led_verde.jpg).
- [Circuito con LED rojo encendido](evidencias/CAMI_led_rojo.jpg).

Las duraciones son las de los archivos audiovisuales, **no la duración total de los ensayos** ni una medición del tiempo de respuesta del sistema.

## 2. Resultados frente al protocolo

El procedimiento previsto está descrito en [`Documentación Técnica/PROTOCOLO_PRUEBAS_CAMI.md`](../Documentaci%C3%B3n%20T%C3%A9cnica/PROTOCOLO_PRUEBAS_CAMI.md).

| Caso | Resultado sustentado por los videos | Estado de verificación | Evidencia |
|---|---|---|---|
| P1 — Encendido y lectura continua | La electrónica aparece alimentada, pero no se visualiza una secuencia de valores ADC. | **Parcial:** no se verifican continuidad ni tasas de muestreo. | V2 |
| P2 — Dashboard local | No se ve el dashboard ni su actualización en pantalla. | **Pendiente** de captura real. | — |
| P3 — Registro persistente | No hay CSV ni registro visible que permita comprobar almacenamiento tras reiniciar. | **Pendiente**. | — |
| P4 — ThingSpeak | No se visualiza el canal con datos atribuibles a esta prueba. | **Pendiente**. | — |
| P5 — Semáforo y buzzer | En V2 se observan estados luminosos distintos, incluido verde encendido y rojo encendido; no se puede atribuir la transición a un umbral ADC específico solo con el video. No se certifica el funcionamiento del buzzer a partir de fotogramas. | **Parcial:** LEDs observables; modo de activación y buzzer por corroborar. | V2 |
| P6 — Encapsulado | V1 muestra el casco puesto con módulo lateral; V2 muestra carcasas y componentes separados durante la prueba. | **Parcial:** falta registro de tres ángulos y apertura/cierre sin daño. | V1, V2 |

## 3. Observaciones verificables de la prueba de mesa

1. **Montaje electrónico:** se aprecia un sensor con carcasa metálica perforada, una placa electrónica, cableado y un módulo con tres indicadores de colores.
2. **Indicador verde:** se observa iluminado en parte del video V2, por ejemplo, cerca del segundo 12.
3. **Indicador rojo:** se observa iluminado posteriormente, por ejemplo, cerca del segundo 18.
4. **Alcance:** la grabación documenta una **secuencia visual de salidas**. Por sí sola no demuestra medición cuantitativa de CO, precisión, detección certificada ni sincronización con el panel web.

### Datos cuantitativos aún no acreditados

| Indicador | Valor registrado |
|---|---|
| Concentración de CO (ppm) | **No medida ni calibrada.** |
| ADC inicial y ADC durante la prueba | No visible en los videos. |
| Umbral efectivamente superado durante la filmación | No verificable en los videos. |
| Tiempo de respuesta al estímulo | No medido. |
| Registros guardados en LittleFS / CSV | Sin evidencia adjunta. |
| Nuevas entradas reales en ThingSpeak | Sin evidencia adjunta. |
| Alarma sonora | Requiere confirmación del equipo y, si es posible, evidencia audible identificable. |

## 4. Fallas, limitaciones y mejoras

**Falla real corregida:** todavía no hay registro suficiente para describir una falla específica, su causa y la corrección efectivamente implementada. Debe agregarse un caso real documentado por el equipo, tal como exige la pauta; no se inventará uno.

**Limitaciones evidenciadas:** estos videos no permiten verificar lecturas del dashboard, almacenamiento ni la correspondencia entre la señal del MQ-7 y las luces. Para reforzar el registro, se recomienda grabar en una misma toma el circuito y el dashboard, o conservar capturas de ambos con hora consistente; también documentar la prueba de buzzer y cualquier falla real resuelta.

## 5. Relación con el problema original

CAMI fue propuesto para explorar un sistema de advertencia de condiciones ambientales potencialmente peligrosas en minería subterránea. Las evidencias disponibles apoyan que el equipo **construyó un montaje de casco y pudo mostrar al menos dos estados luminosos** en el circuito de pruebas. Es una validación parcial de montaje y señalización, no una validación de detección fiable de gases ni de aptitud de uso minero.

El sensor MQ-7 utilizado **no está calibrado aquí para cuantificar CO en ppm** y el firmware del repositorio no implementa el ciclo térmico requerido para una medición validada. Por ello, CAMI es un **prototipo académico** que no debe utilizarse como equipo de seguridad personal.

## 6. Pendientes para cerrar Testing

- [x] Videos de casco y de funcionamiento visual del semáforo disponibles.
- [x] Observaciones reales de los videos documentadas, distinguiendo lo visible de lo no demostrado.
- [ ] Captura del dashboard mostrando **datos reales** del dispositivo.
- [ ] Captura o CSV del historial real y, si aplica, nuevas entradas reales de ThingSpeak.
- [ ] Confirmación del buzzer y condición bajo la que cambian las luces.
- [ ] Fotos del casco completo de frente, lateral y atrás (solo si realmente están disponibles).
- [ ] Una falla realmente encontrada, su corrección y resultado de la repetición.

**Nota de privacidad:** antes de publicar videos o imágenes que muestran personas reconocibles en un repositorio público, confirmar que las personas grabadas aceptan su publicación. Si no, usar fotogramas del hardware sin rostros.
