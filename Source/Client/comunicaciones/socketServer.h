#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <netinet/in.h>
#include <stdbool.h>

typedef struct {
    char ipServidor[16];
    int port;
    int sock;  // Descriptor del socket
    bool isConnected;  // Indica si el servidor está conectado
    struct sockaddr_in serverAddress;  // Dirección del servidor
} SocketServer;

// Constructor y Destructor
SocketServer *SocketServer_create();
void SocketServer_destroy(SocketServer *server);

// Conecta el socket al servidor (llamado explícitamente desde el menú)
void SocketServer_connect(SocketServer *server);

// Métodos para enviar y recibir mensajes
void SocketServer_send(SocketServer *server, const char *message);
int SocketServer_receive(SocketServer *server, char *buffer, int bufferSize);

// Método para reconectar si el servidor está desconectado
void SocketServer_reconnect(SocketServer *server);

#endif // SOCKET_SERVER_H
