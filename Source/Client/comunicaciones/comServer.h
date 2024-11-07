#ifndef COM_SERVER_H
#define COM_SERVER_H

#include "socketServer.h"

// Definición de la estructura ComServer
typedef struct {
    SocketServer *socketServer;
} ComServer;

// Constructor y Destructor
ComServer *ComServer_create(const char *clientType);
void ComServer_destroy(ComServer *server);

// Métodos para enviar y recibir mensajes
void ComServer_sendMessage(ComServer *server, const char *message);
int ComServer_receiveMessage(ComServer *server, char *buffer, int bufferSize);

#endif // COM_SERVER_H
