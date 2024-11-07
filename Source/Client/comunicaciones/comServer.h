#ifndef COM_SERVER_H
#define COM_SERVER_H

#include "socketServer.h"

// Definición de la estructura ComServer
typedef struct {
    SocketServer *socketServer;
    void (*onMessageReceived)(const char *message);  // Callback para manejar mensajes
} ComServer;

// Constructor y Destructor
ComServer *ComServer_create(const char *clientType);
void ComServer_destroy(ComServer *server);

// Métodos para enviar y recibir mensajes
void ComServer_sendMessage(ComServer *server, const char *message);
int ComServer_receiveMessage(ComServer *server, char *buffer, int bufferSize);

// Loop de recepción de mensajes
void *ComServer_messageListeningLoop(void *arg);

#endif // COM_SERVER_H
