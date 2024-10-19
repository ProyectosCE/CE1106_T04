#ifndef COM_SERVER_H
#define COM_SERVER_H

#include "socketServer.h"
#include "jsonProcessor.h"

typedef struct {
    // Estructura interna para manejar los datos de comunicación
    int isRunning;  // Para saber si el servidor está corriendo
    SocketServer *socketServer;  // Puntero a la estructura de SocketServer
    JsonProcessor *jsonProcessor;  // Puntero a la estructura de JsonProcessor
} ComServer;

// Constructor y Destructor
ComServer *ComServer_create();
void ComServer_destroy(ComServer *server);

// Métodos de la clase
void ComServer_start(ComServer *server);
void ComServer_sendMessage(ComServer *server, const char *message);
char *ComServer_getProcessedMessage(ComServer *server);
void ComServer_processIncomingMessage(ComServer *server, const char *message);

#endif // COM_SERVER_H
