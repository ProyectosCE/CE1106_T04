#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

// Declaración de la función para iniciar el cliente WebSocket.
void start_websocket_client(const char *esp32_ip);
void start_websocket_client_thread(const char *esp32_ip);

#endif // WEBSOCKET_CLIENT_H
