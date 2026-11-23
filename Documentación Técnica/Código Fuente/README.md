Código fuente — CAMI

Descripción

El programa principal del prototipo CAMI se encuentra en el archivo CAMI_FINAL_GITHUB.ino, desarrollado para una placa ESP32.

El firmware utiliza un sensor MQ-7 para detectar variaciones en su señal analógica y controlar un sistema de alertas luminosas y sonoras.

Conexiones programadas

Componente	Pin ESP32
Sensor MQ-7 (AO)	GPIO34, mediante divisor de tensión
LED verde	GPIO25
LED amarillo	GPIO26
LED rojo	GPIO27
Buzzer	GPIO23, mediante transistor BC547

Estas conexiones corresponden al programa y deben comprobarse en el circuito físico.

Funcionamiento

El programa contempla tres estados:

* Normal: LED verde encendido.
* Prealerta: LED amarillo encendido.
* Alerta: LED rojo y buzzer activados.

Los umbrales experimentales son aumentos de 120 y 300 unidades ADC respecto de una referencia. No equivalen a concentraciones de CO en ppm.

Dashboard

El programa incluye un panel web local para visualizar lecturas, consultar estados y ejecutar pruebas de las salidas. También incorpora funciones de registro CSV y comunicación con ThingSpeak.

Estas funciones deben verificarse con la ESP32 encendida y conectada al montaje real.

Limitaciones

El sensor no cuenta con calibración validada para determinar concentraciones de CO en ppm. CAMI es un prototipo académico y no un dispositivo de seguridad certificado.
