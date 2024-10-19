#include "ComServer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Constructor: Inicializa la estructura ComServer
 */
ComServer *ComServer_create() {
    ComServer *server = (ComServer *)malloc(sizeof(ComServer));
    if (server == NULL) {
        printf("Error al asignar memoria para ComServer\n");
        return NULL;
    }

    // Inicializar valores
    server->isRunning = 0;
    server->socketServer = SocketServer_create();  // Inicializa el socket server
    server->jsonProcessor = JsonProcessor_create();  // Inicializa el procesador de JSON

    return server;
}

/*
 * Destructor: Libera los recursos de ComServer
 */
void ComServer_destroy(ComServer *server) {
    if (server != NULL) {
        SocketServer_destroy(server->socketServer);  // Libera recursos de SocketServer
        JsonProcessor_destroy(server->jsonProcessor);  // Libera recursos de JsonProcessor
        free(server);
    }
}

/*
 * Método para iniciar la conexión de comunicación
 */
void ComServer_start(ComServer *server) {
    if (server == NULL) {
        printf("Servidor no inicializado.\n");
        return;
    }
    server->isRunning = 1;
    SocketServer_start(server->socketServer);  // Inicia la conexión de socket
}

/*
 * Método para enviar mensajes al servidor
 */
void ComServer_sendMessage(ComServer *server, const char *message) {
    if (server == NULL) {
        printf("Servidor no inicializado.\n");
        return;
    }

    // Convertir el mensaje a JSON utilizando JsonProcessor
    char *jsonMessage = JsonProcessor_createJsonMessage(server->jsonProcessor, message);

    // Enviar el mensaje al servidor usando el SocketServer
    SocketServer_send(server->socketServer, jsonMessage);

    // Liberar memoria
    free(jsonMessage);
}

/*
 * Método para recibir mensajes procesados desde el servidor
 */
char *ComServer_getProcessedMessage(ComServer *server) {
    if (server == NULL) {
        printf("Servidor no inicializado.\n");
        return NULL;
    }

    // Recibir mensaje del servidor
    char buffer[1024];
    int bytesReceived = SocketServer_receive(server->socketServer, buffer, sizeof(buffer));

    if (bytesReceived > 0) {
        // Procesar el mensaje JSON
        return JsonProcessor_processJsonMessage(server->jsonProcessor, buffer);
    }

    return NULL;
}

/*
 * Método para procesar mensajes entrantes desde el servidor
 */
void ComServer_processIncomingMessage(ComServer *server, const char *message) {
    if (server == NULL) {
        printf("Servidor no inicializado.\n");
        return;
    }

    // Procesar el mensaje JSON recibido
    char *processedMessage = JsonProcessor_processJsonMessage(server->jsonProcessor, message);

    // Mostrar el mensaje procesado (Simular que lo devuelve al main)
    printf("Mensaje recibido del servidor y procesado: %s\n", processedMessage);

    // Liberar memoria
    free(processedMessage);
}