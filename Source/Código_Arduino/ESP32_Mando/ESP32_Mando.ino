/*
================================== LICENCIA ==================================================
MIT License
Copyright (c) 2024 José Bernardo Barquero Bonilla,
                   Jose Eduardo Campos Salazar,
                   Jimmy Feng Feng,
                   Alexander Montero Vargas
Consulta el archivo LICENSE para más detalles.
==============================================================================================
*/
#include <WiFi.h>
#include <WebSocketsServer.h>

// Configuración de la red WiFi
const char* ssid = "amv"; // Reemplaza con tu SSID
const char* password = "pass1234"; // Reemplaza con tu contraseña

// Servidor WebSocket
WebSocketsServer webSocket = WebSocketsServer(81);

// Pines del joystick
const int xPin = 32; // Eje X
const int yPin = 34; // Eje Y
const int buttonPin = 22; // Botón

/* Function: setup
   Configura el microcontrolador para establecer la conexión WiFi, iniciar el servidor WebSocket y configurar los pines del joystick.

   Params:
   - Ninguno.

   Returns:
   - void - No retorna ningún valor.

   Restriction:
   Esta función debe ser llamada una vez al inicio del programa para inicializar correctamente los recursos necesarios.

   Example:
   setup();
*/
void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);

  // Conexión a la red WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Inicia el servidor WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

/* Function: loop
   Se ejecuta continuamente para leer los valores del joystick y enviar mensajes a los clientes conectados mediante WebSocket.

   Params:
   - Ninguno.

   Returns:
   - void - No retorna ningún valor.

   Restriction:
   La función depende de la estabilidad de la conexión WiFi y el servidor WebSocket iniciado previamente en `setup`.

   Example:
   // El sistema Arduino ejecuta automáticamente esta función en bucle.
*/
void loop() {
  webSocket.loop();

  // Leer valores del joystick
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  bool buttonPressed = !digitalRead(buttonPin);

  Serial.println();
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Verificar y enviar datos por WebSocket
  if (xValue > 1975) {
    webSocket.broadcastTXT("Derecha");
  } else if (xValue < 1825) {
    webSocket.broadcastTXT("Izquierda");
  }

  if (yValue > 1975) {
    webSocket.broadcastTXT("Abajo");
  } else if (yValue < 1825) {
    webSocket.broadcastTXT("Arriba");
  }

  if (buttonPressed) {
    webSocket.broadcastTXT("Presionado");
  }

  delay(100); // Pequeño retraso para estabilidad
}

/* Function: webSocketEvent
   Maneja los eventos del servidor WebSocket. En este caso, no realiza ninguna acción específica porque el servidor solo envía datos.

   Params:
   - num: uint8_t - ID del cliente que generó el evento.
   - type: WStype_t - Tipo de evento WebSocket (e.g., conexión, mensaje, desconexión).
   - payload: uint8_t* - Datos recibidos del cliente.
   - length: size_t - Longitud del payload.

   Returns:
   - void - No retorna ningún valor.

   Restriction:
   Actualmente, esta función está vacía, pero puede modificarse para manejar eventos personalizados si es necesario.

   Example:
   webSocketEvent(0, WStype_TEXT, (uint8_t*)"Hola", 4);
*/
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  // Evento WebSocket, se deja vacío porque solo enviamos datos
}