# CAMI — Descripción del circuito actualizado (MQ-7)

**Proyecto:** CAMI — Casco inteligente de monitoreo ambiental  
**Referencia de implementación:** `CAMI_FINAL_GITHUB.ino`, versión almacenada en el repositorio el 23-11-2026.  
**Esquema gráfico asociado:** [ESQUEMATICO_CAMI_MQ7.pdf](ESQUEMATICO_CAMI_MQ7.pdf)

## 1. Función del circuito

El sistema utiliza una **ESP32 DevKit V1** para adquirir una señal analógica del módulo **MQ-7** y clasificar cambios de señal respecto a una referencia experimental. Activa indicadores verde, amarillo y rojo y controla un buzzer mediante un **BC547**. En software se incluye un dashboard local y funciones de almacenamiento y comunicación con ThingSpeak.

El sensor **MQ-7 sustituye al MQ-135** de una etapa anterior. El archivo histórico `Diagrama.jpeg` representa **otro montaje** y no debe presentarse como esquemático actualizado.

## 2. Tabla de conexiones de referencia

| Elemento | Conexión descrita en el firmware/documentación | Función |
|---|---|---|
| MQ-7, VCC | 5 V regulados después de interruptor S1 | Alimentación del módulo del sensor |
| MQ-7, GND | GND común | Retorno eléctrico |
| MQ-7, AO | R1 10 kΩ → nodo de lectura → GPIO34 | Señal analógica hacia ADC de ESP32 |
| R2 10 kΩ | Nodo GPIO34 → GND | Divisor de tensión 1:2 con R1 |
| MQ-7, DO | Sin conectar | Salida digital no utilizada |
| LED verde del semáforo | GPIO25, por resistencia limitadora integrada en módulo | Indicador normal |
| LED amarillo del semáforo | GPIO26, por resistencia limitadora integrada en módulo | Indicador prealerta |
| LED rojo del semáforo | GPIO27, por resistencia limitadora integrada en módulo | Indicador alerta |
| Retorno módulo semáforo | GND común | Retorno de indicadores |
| GPIO23 | R3 1 kΩ → base del BC547 | Mando del buzzer |
| BC547 emisor | GND común | Retorno del transistor |
| BC547 colector | Negativo del buzzer | Conmutación del buzzer |
| Buzzer, positivo | 5 V regulados después de S1 | Alimentación del buzzer |

## 3. Control implementado en el programa

- **GPIO34:** lee la señal analógica; el programa utiliza un valor filtrado de ADC, referencia inicial y un indicador de aumento de señal.
- **GPIO25 / GPIO26 / GPIO27:** controlan las luces verde, amarilla y roja, respectivamente.
- **GPIO23:** activa el buzzer (en el código actual, `BUZZER_ACTIVO = false` y se genera un tono para buzzer pasivo).
- **Umbrales de demostración:** aumento de ADC de **120** para prealerta y **300** para alerta, con histéresis de **30** unidades ADC. **No son ppm de CO.**
- **Modo de prueba manual del firmware:** incluye secuencia verde → amarillo → rojo con buzzer; su ejecución física requiere evidencia aparte.

## 4. Alimentación y comprobaciones antes de tratarlo como esquema validado

El circuito de señal y control se dibuja a partir del código y la documentación ya entregados, **pero la fuente de 5 V, regulación/protección de las celdas 18650, la posición física del interruptor S1, la tensión nominal del buzzer y el conexionado real deben contrastarse con el prototipo**. El PDF deja la fuente como bloque expresamente **no verificado**; no recomienda conectar dos celdas 18650 directamente al pin de 5 V de la ESP32.

Verificar especialmente: identificación de terminales B/C/E según la orientación del BC547 físico; niveles máximos admisibles en GPIO34; resistencias limitadoras realmente incorporadas en el módulo semáforo; masa común y polaridades. El esquemático describe la intención eléctrica documentada, **no sustituye una inspección del cableado**.

## 5. Alcance experimental

Este montaje académico **no mide CO calibrado en ppm** y no implementa en el firmware el control térmico requerido para una caracterización fiable del MQ-7. No utilizar CAMI como detector de CO certificado ni para determinar si una atmósfera es segura.
