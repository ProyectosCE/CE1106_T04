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

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  // Evento WebSocket, se deja vacío porque solo enviamos datos
}