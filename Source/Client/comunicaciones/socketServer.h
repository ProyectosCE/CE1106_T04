#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <netinet/in.h>
#include <stdbool.h>

typedef struct {
    char ipServidor[16];
    int port;
    int sock;  // Descriptor del socket
    bool isConnected;  // Bool para conocer si el servidor está activado o no
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
