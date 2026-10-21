# Evidencia de Iteración y Testing

## Resolución parcial del problema
La problemática se relaciona principalmente con el Objetivo de Desarrollo Sostenible 8: Trabajo decente y crecimiento económico, el cual promueve, entre otros aspectos,
condiciones laborales seguras y la protección de los trabajadores. En minería subterránea, la exposición a concentraciones peligrosas de monóxido de carbono representa un riesgo directo para la seguridad laboral. Por ello, mejorar la capacidad de detectar y advertir oportunamente su presencia contribuye al desarrollo de entornos de trabajo más seguros.
De manera complementaria, el proyecto se vincula con el Objetivo de Desarrollo Sostenible 3: Salud y bienestar, orientado a proteger la salud de las personas y reducir factores que puedan afectarla negativamente. La exposición a monóxido de carbono puede provocar intoxicaciones y consecuencias graves para la salud, por lo que su detección oportuna puede contribuir a disminuir la exposición de los trabajadores. De esta manera, ambos ODS se relacionan directamente con la problem´atica: el ODS 8 desde
la necesidad de garantizar condiciones laborales seguras y el ODS 3 desde la protección de la salud y el bienestar de los trabajadores frente a la exposición al monóxido de carbono.

## Problemas y soluciones:
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

### - El buzzer no sonaba con una salida constante.
Se probó una señal de frecuencia en modo pasivo. El buzzer comenzó a sonar, aunque con volumen bajo.

