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
#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <netinet/in.h>

typedef struct {
    const char* ipServidor;  // Apunta al string de la dirección IP
    int port;               // Cambiar a un puntero para el puerto
    int sock;               // Descriptor del socket
    bool isConnected;       // Bool para conocer si el servidor está activado o no
    struct sockaddr_in serverAddress;  // Dirección del servidor
} SocketServer;

// Constructor y Destructor
SocketServer *SocketServer_create();
void SocketServer_destroy(SocketServer *server);

// Métodos de la clase
void SocketServer_start(SocketServer *server);
void SocketServer_send(SocketServer *server, const char *message);
int SocketServer_receive(SocketServer *server, char *buffer, int bufferSize);
void SocketServer_reconnect(SocketServer *server);
bool SocketServer_isConnected(SocketServer *server);

#endif // SOCKET_SERVER_H
