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
#ifndef COM_SERVER_H
#define COM_SERVER_H

#include "socketServer.h"
#include "jsonProcessor.h"


// Definición del callback que será llamado cuando se reciba un nuevo mensaje
typedef void (*MessageReceivedCallback)(const char *message);

typedef struct {
    // Estructura interna para manejar los datos de comunicación
    int isRunning;  // Para saber si el servidor está corriendo
    SocketServer *socketServer;  // Puntero a la estructura de SocketServer
    JsonProcessor *jsonProcessor;  // Puntero a la estructura de JsonProcessor
    MessageReceivedCallback onMessageReceived;  // Función callback
} ComServer;

// Constructor y Destructor
ComServer *ComServer_create();
void ComServer_destroy(ComServer *server);

// Métodos de la clase
void ComServer_start(ComServer *server);
char *ComServer_getProcessedMessage(ComServer *server);
void ComServer_processIncomingMessage(ComServer *server, const char *message);

// Métodos para enviar y recibir mensajes
void ComServer_sendMessage(ComServer *server, const char *message);
void ComServer_sendStatus(const char *message);
void ComServer_sendPlayerName(ComServer *server, const char *message);
void ComServer_registerCallback(ComServer *server, MessageReceivedCallback callback);
void *ComServer_messageListeningLoop(void *arg);
void ComServer_observerGetlist(ComServer *server);
void comServer_sendChoosenPlayer(ComServer *server,const char *player);


#endif // COM_SERVER_H
