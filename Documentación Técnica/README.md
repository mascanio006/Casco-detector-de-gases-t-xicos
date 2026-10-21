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

