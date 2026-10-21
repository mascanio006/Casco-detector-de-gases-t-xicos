# Diagramas de circuito


## Pines del ESP32

1-**_Pin:_ 5V**

_A que se conecta:_ VCC del MQ135

_Función:_ Alimentar el módulo sensor

2-**_Pin:_ GND**

_A que se conecta:_ GND del MQ135 y retornos del circuito

_Función:_ Establecer una referencia eléctrica común

3-**_Pin:_  GPIO34 / P34**

_A que se conecta:_ Unión entre las dos resistencias de 10 kΩ

_Función:_ Leer la señal analógica reducida del sensor

4-**_Pin:_ GPIO25 / P25**

_A que se conecta:_ Resistencia de 220 Ω y ánodo del LED

_Función:_  Controlar la alerta visual

5-**_Pin:_ GPIO23 / P23**

_A que se conecta:_ Resistencia de 220 Ω y positivo del buzzer

_Función:_ Generar la señal de alerta sonora

6-**_Pin:_  USB**

_A que se conecta:_ Computador

_Función:_ Alimentar la placa, cargar el programa y consultar el monitor serial

## Componentes utilizados

1- ESP-32
Cantidad: 1
Procesamiento, entradas y salidas, conexión WiFi.

2- Módulo MQ135
Cantidad: 1
Sensor de gases con salida analógica; responde a distintas sustancias presentes en el aire.

3- Protoboard
Cantidad: 1
Montaje y modificación del circuito sin soldadura.

4- LED rojo
Cantidad: 1
Alerta visual continua cuando el programa activa la alarma.

5- Buzzer CEET120B085
Cantidad: 1
Transductor electromagnético que respondió a la excitación en modo pasivo. El sufijo completo del modelo debe verificarse para especificar sus valores nominales.

6- Resistencias de 10 kΩ
Cantidad: 2
Divisor de voltaje entre AO y GPIO34.

7- Resistencias de 220 Ω
Cantidad: 2
Una para limitar la corriente del LED y otra para la conexión de prueba del buzzer.

8- Cables jumper
Cantidad: Varios
Conexiones entre placa, protoboard y componentes.

9- Cable USB
Cantidad: 1
Alimentación y programación.


