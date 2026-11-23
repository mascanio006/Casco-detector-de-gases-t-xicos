# Evidencia del dashboard de CAMI

## Objetivo
Respaldar con evidencia visual el funcionamiento del dashboard del prototipo CAMI durante la demostración final.

## Archivos de origen revisados
- `ScreenRecording_11-23-2026 16-15-05_1.mp4`: grabación de pantalla del dashboard.
- `IMG_5212.mp4`: video externo de la prueba, donde se observa el casco y el dashboard al mismo tiempo.

## Qué se verifica con la evidencia
1. El dashboard local de CAMI se abre correctamente en la dirección `192.168.4.1`.
2. El sistema identifica al casco como conectado.
3. Se observan distintos estados del dispositivo según la señal medida por la ESP32:
   - **Señal bajo el umbral** (verde).
   - **Aumento de señal** (amarillo).
   - **Alerta por aumento** (rojo).
4. Se visualizan variables del sistema como:
   - voltaje AO estimado,
   - señal filtrada en ADC,
   - entradas en rojo,
   - gráfico de tendencia del aumento,
   - panel de lectura y conectividad.
5. En el estado rojo, el propio dashboard indica **Buzzer ON**.
6. En el video externo se aprecia la prueba simultánea del casco y la interfaz mostrada en la tablet.

## Evidencia seleccionada
- `dashboard_estado_verde.jpg`
  - Estado observado: **Señal bajo el umbral**.
  - Lectura visible: voltaje AO estimado **0.284 V**, señal filtrada **0 ADC**, entradas en rojo **0**.

- `dashboard_estado_amarillo.jpg`
  - Estado observado: **Aumento de señal**.
  - Lectura visible: voltaje AO estimado **0.697 V**, señal filtrada **261 ADC**, entradas en rojo **0**.

- `dashboard_estado_rojo.jpg`
  - Estado observado: **Alerta por aumento**.
  - Lectura visible: voltaje AO estimado **2.719 V**, señal filtrada **1540 ADC**, entradas en rojo **1**.
  - El dashboard muestra además **Buzzer ON**.

- `dashboard_tendencia_y_conectividad.jpg`
  - Se observa el gráfico de tendencia y el panel de lectura y conectividad.
  - En la gráfica se aprecia un aumento pronunciado de la señal, con retorno posterior hacia niveles bajos.
  - Se muestra además información del panel local, incluyendo referencia de comparación, aumento absoluto y estado de enlace del dashboard local.

- `dashboard_prueba_simultanea.jpg`
  - Fotograma del video externo en el que se observa simultáneamente el casco y la tablet con el dashboard abierto.

- `dashboard_tabla_y_control.jpg`
  - Fotograma del video externo donde se observa la sección de control y evidencia, junto con la tabla de últimas muestras recibidas.

## Resultado escrito de la prueba del dashboard
Durante la demostración final, el dashboard local del prototipo CAMI mostró datos en tiempo real enviados por la ESP32. En la grabación de pantalla se verificó la transición entre tres estados del sistema: verde (señal bajo el umbral), amarillo (aumento de señal) y rojo (alerta por aumento). En el estado rojo, el dashboard indicó activación del buzzer y un incremento importante de la señal filtrada, alcanzando 1540 ADC con un voltaje AO estimado de 2.719 V. Además, se observó el gráfico de tendencia del aumento y el panel de lectura y conectividad. En el segundo video, grabado externamente, se comprobó que el casco y la interfaz del dashboard estaban operando al mismo tiempo durante la prueba.

## Alcance y limitaciones
- La interfaz permite comprobar cambios de señal y estados del sistema durante la prueba, pero **no constituye una medición calibrada de monóxido de carbono en ppm**.
- La pantalla indica `Buzzer ON` en el estado rojo; el indicador visual no demuestra por sí solo la intensidad ni el funcionamiento acústico del buzzer.
- En la grabación se observa el dashboard local activo, pero **no queda acreditado un envío exitoso a ThingSpeak**; el panel muestra ausencia de Wi-Fi con internet y de envío confirmado.
- El video externo documenta casco y tablet al mismo tiempo. Los dos videos de origen pueden añadirse como evidencia complementaria en `testing/evidencias/`.

## Redacción breve sugerida para el repositorio
> Se incorporó evidencia real del dashboard de CAMI mediante una grabación de pantalla y un video externo de la prueba. La interfaz mostró correctamente los estados “Señal bajo el umbral”, “Aumento de señal” y “Alerta por aumento”, junto con las variables de monitoreo y el gráfico de tendencia. También se verificó visualmente la operación simultánea del casco y la tablet con el dashboard abierto.
