# C A M I
CAMI (Casco de Ayuda Minera Inmediata) es un sistema loT especialmente diseñado para trabajadores del sector minero que ejercen su labor en minas subterráneas. El casco posee un sensor integrado que mide, en tiempo real, la presencia de monóxido de carbono. Si la medición cumple con la condición de alerta programada, el casco emitirá una vibración, un ruido y una luz LED para advertirle al minero que se encuentra en una circunstancia crítica.
## Equipo
- Mariana Ascanio

- Antonia Briceño

- Emilia Echavarria

- Ainara Felix

- Andres Obreque


## Descripción del problema
Cuando hay demasiado monóxido de carbono en el aire, este gas reemplaza el oxígeno en los glóbulos rojos. Esto puede derivar en un daño grave en los tejidos o incluso en la muerte. Este gas es de alto riesgo ya que no tiene sabor, olor, ni color. La intoxicación de monóxido de carbono en personas mayores (los cuales conforman la mayor parte del personal obrero en el ámbito minero) puede derivar en daño cerebral.

La problemática se relaciona principalmente con el **Objetivo de Desarrollo Sostenible 8: Trabajo decente y crecimiento económico**, el cual promueve, entre otros aspectos,
condiciones laborales seguras y la protección de los trabajadores. En minería subterránea, la exposición a concentraciones peligrosas de monóxido de carbono representa un riesgo directo para la seguridad laboral. Por ello, mejorar la capacidad de detectar y advertir oportunamente su presencia, contribuye al desarrollo de entornos de trabajo más seguros.
De manera complementaria, el proyecto se vincula con el **Objetivo de Desarrollo Sostenible 3: Salud y bienestar**, orientado a proteger la salud de las personas y reducir factores que puedan afectarla negativamente. La exposición a monóxido de carbono puede provocar intoxicaciones y consecuencias graves para la salud, por lo que su detección oportuna puede contribuir a disminuir la exposición de los trabajadores. De esta manera, ambos ODS se relacionan directamente con la problemática: el ODS 8 desde
la necesidad de garantizar condiciones laborales seguras y el ODS 3 desde la protección de la salud y el bienestar de los trabajadores frente a la exposición al monóxido de carbono.


## Estructura del repositorio
- Conceptualización del Encapsulado
   > Modelo 3D
   
- Documentación Técnica
   > Código Fuente
   
   > Diagramas de Circuito

En el avance presentamos el diseño, fabricación y encapsulado del sensor del casco. 

## Dashboard IoT – ThingSpeak

Para el monitoreo remoto de CAMI se creó un canal privado en ThingSpeak, plataforma que permite almacenar y visualizar datos enviados por dispositivos IoT.

El dashboard incluye:

* Field 1: Voltaje del sensor (V).
* Field 2: Lectura analógica del sensor (ADC).
* Field 3: Estado de alerta (0 = normal, 1 = precaución, 2 = alarma).

Además, se implementaron tres indicadores visuales: verde para estado normal, amarillo para precaución y rojo para alarma.

### Pruebas realizadas

Se realizaron pruebas manuales mediante la API de ThingSpeak para verificar la recepción, almacenamiento y visualización de datos.

Se comprobó el funcionamiento del indicador de voltaje con un valor simulado de 2,5 V y del indicador ADC con un valor simulado de 1800.

**Estado actual**: El dashboard funciona con datos de prueba. Está pendiente su integración con las mediciones reales de la ESP32 y la validación de los umbrales de alerta.

