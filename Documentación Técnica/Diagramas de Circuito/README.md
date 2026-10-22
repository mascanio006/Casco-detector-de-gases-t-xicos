# Diagrama de circuito

![imagen alt](https://github.com/mascanio006/Casco-detector-de-gases-t-xicos/blob/5954e6161be8534786e09ebd8e5f0c46f2082786/Documentaci%C3%B3n%20T%C3%A9cnica/Diagramas%20de%20Circuito/Diagrama.jpeg)

Foto 1: Diagrama de circuito del sensor

El prototipo utiliza un sensor MQ135 conectado a una placa ESP32 para detectar variaciones en su señal ante la presencia de humo. El ESP32 procesa la lectura y controla dos salidas: un LED rojo de alerta y un buzzer pasivo. Además, transmite los datos a una página web mediante una red WiFi propia.

El circuito se montó en un protoboard con cables jumper. Esta decisión permitió modificar conexiones y reemplazar componentes durante las pruebas, sin soldar el sistema.

### Adaptación de la señal del sensor

El MQ135 se alimenta a 5 V, mientras que la entrada del ESP32 requiere una señal compatible con su nivel de 3,3 V. Para reducir el voltaje de AO se utilizaron dos resistencias iguales de 10 kΩ.

### La conexión es:

AO → primera resistencia → unión hacia GPIO34 → segunda resistencia → GND.

El divisor reduce aproximadamente a la mitad el voltaje de entrada:

V_GPIO34 = V_AO × 10 kΩ / (10 kΩ + 10 kΩ).

Por ejemplo, si AO entrega 5 V, GPIO34 recibe aproximadamente 2,5 V.

### Conexión de los actuadores

El LED se conectó desde GPIO25 mediante una resistencia de 220 Ω hasta su ánodo —pata larga—. Su cátodo —pata corta— se conectó a GND. En la última versión del programa, permanece apagado durante la espera inicial y se enciende de forma continua durante la alerta.
Evidencia y limitaciones del montaje

El volumen del buzzer quedó como una limitación pendiente. Se propuso incorporar una etapa con transistor y protección para manejarlo adecuadamente; esa mejora no se incluye como implementada en este diagrama.

Finalmente, el montaje demuestra una respuesta a cambios en la señal del MQ135. La lectura no permite afirmar una concentración real de CO en ppm ni certificar que el aire sea seguro. Aunque la interfaz de la fotografía muestre “CO”, esa etiqueta no valida la medición.

## Componentes utilizados

1- **ESP-32**

Cantidad: 1

Función: procesamiento, entradas y salidas, conexión WiFi.


2- **Módulo MQ135**

Cantidad: 1

Función: sensor de gases con salida analógica; responde a distintas sustancias presentes en el aire.

3- **Protoboard**

Cantidad: 1

Función: montaje y modificación del circuito sin soldadura.

4- **LED rojo**

Cantidad: 1

Función: alerta visual continua cuando el programa activa la alarma.

5- **Buzzer CEET120B085**

Cantidad: 1

Función: transductor electromagnético que respondió a la excitación en modo pasivo. El sufijo completo del modelo debe verificarse para especificar sus valores nominales.

6- **Resistencias de 10 kΩ**

Cantidad: 2

Función: divisor de voltaje entre AO y GPIO34.


7- **Resistencias de 220 Ω**

Cantidad: 2

Función: una para limitar la corriente del LED y otra para la conexión de prueba del buzzer.

8- **Cables jumper**

Cantidad: Varios

Función: conexiones entre placa, protoboard y componentes.

9- **Cable USB**

Cantidad: 1

Función: alimentación y programación.

## Pines del ESP32

1-**_Pin:_ 5V**

A que se conecta: VCC del MQ135.

Función: alimentar el módulo sensor.

2-**_Pin:_ GND**

A que se conecta: GND del MQ135 y retornos del circuito.

Función: establecer una referencia eléctrica común.

3-**_Pin:_  GPIO34 / P34**

A que se conecta: unión entre las dos resistencias de 10 kΩ.

Función: leer la señal analógica reducida del sensor.

4-**_Pin:_ GPIO25 / P25**

A que se conecta: resistencia de 220 Ω y ánodo del LED.

Función: controlar la alerta visual.

5-**_Pin:_ GPIO23 / P23**

A que se conecta: resistencia de 220 Ω y positivo del buzzer.

Función: generar la señal de alerta sonora.

6-**_Pin:_  USB**

A que se conecta: computador.

Función: alimentar la placa, cargar el programa y consultar el monitor serial.

## Cambios e iteraciones:
### _El indicador POWER del MQ135 no encendía_
Se conectó VCC y GND directamente al ESP32 mediante cables hembra-hembra. POWER encendió. La prueba permitió localizar el problema en el recorrido anterior de alimentación, sin demostrar que todo el protoboard estuviera defectuoso.

### _Necesidad de modificar el montaje y cambiar el sensor en el futuro_
Se utilizó protoboard en lugar de soldar.
Se facilitaron las pruebas y sustituciones.

### _Conexiones difíciles de organizar_
Se redistribuyeron componentes y cables durante el armado. Se reforzó la identificación de filas, nodos y tierra común.

### _Necesidad de limitar la corriente del LED_
Se incorporó una resistencia de 220 Ω en serie. Se limitó la corriente que atraviesa el LED.

### _Diferencia entre la alimentación del sensor y el nivel de entrada del ESP32_
Se incorporó el divisor de dos resistencias de 10 kΩ. Se redujo la señal analógica antes de ingresar a GPIO34.

### _El LED se encendía durante el calentamiento y emitía destellos sin alerta_
Se modificó el programa para encenderlo de forma continua únicamente durante la alerta. La indicación visual quedó asociada al estado de alarma.

### _La detección del programa respondía lentamente_
Se redujo el intervalo de lectura, se eliminó el filtro lento y se ajustó el umbral experimental. La última versión entregada evalúa la señal cada 100 ms y conserva la alerta durante al menos 3 segundos. Estos cambios no eliminan el tiempo de respuesta físico del sensor.

### _El buzzer no sonaba con una salida constante_
Se probó una señal de frecuencia en modo pasivo. El buzzer comenzó a sonar, aunque con volumen bajo.
