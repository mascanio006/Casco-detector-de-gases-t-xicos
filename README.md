CAMI — Casco inteligente de monitoreo ambiental

Proyecto académico de Taller de Introducción a la Ingeniería (TEI201).

CAMI es un prototipo de casco con un sistema electrónico de monitoreo de variaciones en la señal de un sensor MQ-7. Una placa ESP32 procesa la lectura y controla indicadores luminosos y una alerta sonora. El firmware también incluye un panel web local y funciones para registrar datos y enviarlos a ThingSpeak.

Importante: CAMI es un prototipo experimental. El sistema no entrega concentraciones de CO calibradas en ppm, no verifica por sí solo que el aire sea seguro y no debe utilizarse como detector de seguridad personal.

Componentes y arquitectura

* Control y sensor: ESP32 y módulo MQ-7; lectura analógica a través de un divisor de tensión.
* Alimentación: módulo de baterías recargables, cuya integración eléctrica definitiva debe verificarse.
* Alertas: luces indicadoras y buzzer; su instalación y funcionamiento real deben comprobarse en el prototipo.
* Carcasas: tres cajas y sus tapas, modeladas en Autodesk Fusion 360 para impresión 3D.

Indicadores programados

Indicador	Significado en el firmware
Verde	Estado normal, señal bajo el umbral experimental
Amarillo	Prealerta por aumento de señal; también parpadea durante el inicio o ante una falla de señal
Rojo + buzzer	Alerta por aumento de señal

Los umbrales se expresan como aumentos en unidades ADC, no como límites de concentración de monóxido de carbono. La indicación de las salidas en el panel web refleja órdenes del programa, no una prueba eléctrica de que las luces o el buzzer estén funcionando.

Archivos del proyecto

* CAMI_FINAL_GITHUB.ino: firmware actual de referencia para ESP32 y MQ-7.
* DOCUMENTACION_ACTUALIZADA.md: rediseño físico y pendientes técnicos.
* Documentación Técnica/Planos Acotados: seis planos técnicos en PDF.
* Documentación Técnica/Código Fuente: explicación del firmware.
* Documentación Técnica/Diagramas de Circuito: documentación del circuito; el diagrama existente corresponde a una versión anterior y requiere actualización.
* Conceptualización del Encapsulado: imágenes y desarrollo de la carcasa.
* docs: capturas y documentación del dashboard.
* FUENTES.md: referencias y declaración de herramientas utilizadas.

Los archivos .f3d en la raíz corresponden a los modelos editables de las cajas y tapas.

Software y conectividad

El firmware incluye lectura analógica del MQ-7, estados normal/prealerta/alerta, control de salidas, servidor web local en la ESP32, almacenamiento CSV y rutinas de envío a ThingSpeak.

La presencia de estas funciones en el código no confirma que hayan sido verificadas en hardware.

Las credenciales Wi-Fi y la clave de escritura de ThingSpeak deben configurarse solo en una copia local, nunca publicarse en el repositorio.

Pruebas y tareas pendientes

1. Verificar las conexiones finales y reemplazar el diagrama anterior del MQ-135 por el del montaje MQ-7.
2. Comprobar las tensiones de alimentación, las protecciones de baterías y la compatibilidad del buzzer.
3. Verificar físicamente la secuencia verde → amarillo → rojo con buzzer, y registrar evidencia.
4. Confirmar la instalación del interruptor y la fijación de las tres carcasas al casco.
5. Comprobar el panel web local, el registro CSV y, si se utiliza, la transmisión real a ThingSpeak.
6. Adjuntar fotografías, resultados y observaciones de las pruebas efectivamente realizadas.

⸻

Documentación del prototipo académico CAMI. No representa un equipo de protección o detección de gases certificado.
