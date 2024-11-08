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
#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

// Declaración de la función para iniciar el cliente WebSocket.
void start_websocket_client(const char *esp32_ip);
void start_websocket_client_thread(const char *esp32_ip);

#endif // WEBSOCKET_CLIENT_H
