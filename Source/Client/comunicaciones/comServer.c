#include "comServer.h"
#include "socketServer.h"
#include "jsonProcessor.h"
#include <log.h>
#include <stdlib.h>
#include <string.h>

// Estructura estática para la única instancia de ComServer
static ComServer *comServer_instance = NULL;

/*
 * Constructor: Inicializa la estructura ComServer y envía el tipo de cliente
 */
ComServer *ComServer_create(const char *clientType) {
    if (comServer_instance != NULL) {
        return comServer_instance;
    }

    // Crear la instancia de ComServer
    comServer_instance = (ComServer *)malloc(sizeof(ComServer));
    if (comServer_instance == NULL) {
        log_fatal("Error al asignar memoria para ComServer\n");
        return NULL;
    }

    // Crear y conectar el socket del servidor
    comServer_instance->socketServer = SocketServer_create();
    if (comServer_instance->socketServer == NULL) {
        free(comServer_instance);
        comServer_instance = NULL;
        return NULL;
    }

    SocketServer_connect(comServer_instance->socketServer);

    // Usar JsonProcessor para crear el mensaje inicial
    char *initMessage = JsonProcessor_createClientTypeMessage(clientType);
    if (initMessage != NULL) {
        SocketServer_send(comServer_instance->socketServer, initMessage);
        free(initMessage);
    } else {
        log_error("Error al crear el mensaje de tipo de cliente.\n");
    }

    return comServer_instance;
}

/*
 * Destructor: Libera los recursos de ComServer
 */
void ComServer_destroy(ComServer *server) {
    if (server != NULL) {
        SocketServer_destroy(server->socketServer);
        free(server);
        comServer_instance = NULL;
    }
}

/*
 * Enviar un mensaje al servidor
 */
void ComServer_sendMessage(ComServer *server, const char *message) {
    if (server != NULL && server->socketServer != NULL) {
        SocketServer_send(server->socketServer, message);
    }
}

/*
 * Recibir un mensaje del servidor
 */
int ComServer_receiveMessage(ComServer *server, char *buffer, int bufferSize) {
    if (server != NULL && server->socketServer != NULL) {
        return SocketServer_receive(server->socketServer, buffer, bufferSize);
    }
    return -1;
}
