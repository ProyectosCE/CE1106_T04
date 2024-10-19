#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <netinet/in.h>

typedef struct {
    int sock;  // Descriptor del socket
    struct sockaddr_in serverAddress;  // Dirección del servidor
} SocketServer;

// Constructor y Destructor
SocketServer *SocketServer_create();
void SocketServer_destroy(SocketServer *server);

// Métodos de la clase
void SocketServer_start(SocketServer *server);
void SocketServer_send(SocketServer *server, const char *message);
int SocketServer_receive(SocketServer *server, char *buffer, int bufferSize);

#endif // SOCKET_SERVER_H
