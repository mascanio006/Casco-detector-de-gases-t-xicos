# FUENTES — CAMI

## Herramientas y documentación consultada / utilizada

- Arduino: lenguaje y entorno de programación. https://docs.arduino.cc/language-reference/
- Espressif Systems: documentación de ESP32. https://docs.espressif.com/projects/esp-idf/en/stable/esp32/
- MathWorks ThingSpeak: documentación de canales, campos y API. https://www.mathworks.com/help/thingspeak/
- Autodesk Fusion: herramienta de modelado de las carcasas del prototipo. https://www.autodesk.com/products/fusion-360/overview
- GitHub: control y publicación de archivos del proyecto. https://docs.github.com/

## Bibliotecas presentes en el firmware entregado

El archivo `CAMI_FINAL_GITHUB.ino` incluye las cabeceras `Arduino.h`, `WiFi.h`, `WebServer.h`, `WiFiClientSecure.h`, `HTTPClient.h`, `LittleFS.h`, `time.h`, `esp_system.h`, `freertos/FreeRTOS.h`, `freertos/task.h` y `freertos/queue.h`. El código indica que utiliza bibliotecas del núcleo ESP32, sin dependencias externas adicionales declaradas.

## Uso de inteligencia artificial

Se utilizó ChatGPT (OpenAI) como apoyo en tareas de documentación y revisión conceptual, entre ellas:

1. Configuración de los tres campos del dashboard ThingSpeak (voltaje, ADC y estado).
2. Explicación de la diferencia entre una señal analógica y una concentración calibrada de CO.
3. Revisión explicativa del firmware y sus posibilidades de integración con ThingSpeak.
4. Organización y redacción de README, documentación de las tres carcasas y listado de fuentes para GitHub.

**Las sugerencias de IA requieren revisión del equipo; no constituyen pruebas de funcionamiento.**

## Fuentes por completar según el trabajo real del equipo

Agregar, si se utilizaron, fichas técnicas específicas del MQ-7, módulo de baterías y buzzer, tutoriales, bibliotecas externas, modelos 3D de terceros, y otras herramientas de IA con sus usos. No se inventan referencias no aportadas por el equipo.
