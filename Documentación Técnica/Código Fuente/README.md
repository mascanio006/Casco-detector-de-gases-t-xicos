Diagramas de circuito — CAMI

Evolución del prototipo

El archivo Diagrama.jpeg corresponde a una versión anterior del circuito CAMI, que utilizaba un sensor MQ-135, un LED rojo y un buzzer.

Este diagrama se conserva como evidencia del desarrollo del proyecto, pero no representa el circuito definitivo.

Versión actual

El firmware CAMI_FINAL_GITHUB.ino contempla una ESP32, un sensor MQ-7, tres indicadores luminosos y un buzzer.

Las conexiones descritas en el programa son:

* MQ-7 AO: divisor de tensión con resistencias de 10 kΩ hacia GPIO34.
* MQ-7 VCC: alimentación de 5 V.
* MQ-7 GND: tierra común.
* Luz verde: GPIO25.
* Luz amarilla: GPIO26.
* Luz roja: GPIO27.
* Buzzer: GPIO23, mediante una etapa de control con transistor BC547.

Verificación pendiente

El equipo debe incorporar el diagrama actualizado del montaje real, incluyendo alimentación, interruptor, resistencias, buzzer y conexiones a tierra.

Las conexiones indicadas anteriormente provienen del firmware y no constituyen una verificación del cableado físico.

El proyecto corresponde a un prototipo académico y no a un sistema de detección de gases certificado.
