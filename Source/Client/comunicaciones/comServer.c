#include "comServer.h"

#include <log.h>

#include "../logs/saveLog.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Puntero estático para almacenar la única instancia de ComServer
static ComServer *comserver_instance = NULL;

/*
 * Constructor: Inicializa el servidor de comunicaciones
 */
ComServer *ComServer_create() {
    if (comserver_instance != NULL) {
        return comserver_instance;
    }

    comserver_instance = (ComServer *)malloc(sizeof(ComServer));
    if (comserver_instance == NULL) {
        savelog_error("Error al crear el servidor de comunicaciones.\n");
        return NULL;
    }

    comserver_instance->socketServer = SocketServer_create();
    comserver_instance->jsonProcessor = JsonProcessor_create();
    comserver_instance->onMessageReceived = NULL;  // Callback inicializado a NULL

    if (comserver_instance->socketServer == NULL || comserver_instance->jsonProcessor == NULL) {
        ComServer_destroy(comserver_instance);
        return NULL;
    }

    SocketServer_start(comserver_instance->socketServer);

    return comserver_instance;
}

/*
 * Destructor: Libera los recursos del servidor de comunicaciones
 */
void ComServer_destroy(ComServer *server) {
    if (server != NULL) {
        SocketServer_destroy(server->socketServer);
        JsonProcessor_destroy(server->jsonProcessor);
        free(server);
        comserver_instance = NULL;
    }
}

/*
 * Método para enviar mensajes al servidor
 */
void ComServer_sendMessage(ComServer *server, const char *message) {
    if (server == NULL) {
        savelog_warn("Servidor no inicializado.\n");
        return;
    }

    char *jsonMessage = JsonProcessor_createJsonMessage(server->jsonProcessor, message);
    savelog_info(jsonMessage);
    SocketServer_send(server->socketServer, jsonMessage);
    free(jsonMessage);
}

void ComServer_sendPlayerName(ComServer *server, const char *name) {
    if (server == NULL) {
        savelog_warn("Servidor no inicializado.\n");
        return;
    }

    char *jsonMessage = JsonProcessor_createJsonPlayerName(server->jsonProcessor, name);
    SocketServer_send(server->socketServer, jsonMessage);
    free(jsonMessage);
}

void ComServer_observerGetlist(ComServer *server) {
    if (server == NULL) {
        savelog_warn("Servidor no inicializado.\n");
        return;
    }

    char *jsonMessage = JsonProcessor_createJsonGetListPlayers(server->jsonProcessor);
    SocketServer_send(server->socketServer, jsonMessage);
    free(jsonMessage);
}

void comServer_sendChoosenPlayer(ComServer *server,const char *playerId) {
        if (server == NULL) {
            savelog_warn("Servidor no inicializado.\n");
            return;
        }

        char *jsonMessage = JsonProcessor_createJsonChoosenPlayer(server->jsonProcessor,playerId);
        SocketServer_send(server->socketServer, jsonMessage);
        free(jsonMessage);

}

void ComServer_sendStatus(const char *message) {
    if (comserver_instance == NULL) {
        savelog_warn("Servidor no inicializado.\n");
        return;
    }
    SocketServer_send(comserver_instance->socketServer, message);
    free(message);
}

/*
 * Método para registrar un callback (Observer) para recibir notificaciones
 */
void ComServer_registerCallback(ComServer *server, MessageReceivedCallback callback) {
    if (server != NULL) {
        printf("Se registro el callback corectamente\n");
        server->onMessageReceived = callback;
    }
    else {
        printf("UPS, no se puedo registrar el callback");
    }
}

/*
 * Método que ejecuta el loop de recepción de mensajes desde el servidor
 */
void *ComServer_messageListeningLoop(void *arg) {
    ComServer *server = (ComServer *)arg;
    if (server == NULL) {
        savelog_error("Servidor no inicializado para la escucha de mensajes.\n");
        return NULL;
    }

    char buffer[4096];
    while (1) {
        if (server->socketServer->isConnected) {
            int bytesReceived = SocketServer_receive(server->socketServer, buffer, sizeof(buffer));
            if (bytesReceived > 0) {
                if (server->onMessageReceived != NULL) {
                    server->onMessageReceived(buffer);  // Notificar al observer
                }
                //free(processedMessage);
            } else {
                log_error("Error al recibir el mensaje del servidor\n");
            }
        } else {
            SocketServer_reconnect(server->socketServer);
        }
    }
}

/*
 * Método para manejar la recepción de mensajes
 */
void ComServer_handleReceivedMessage(ComServer *server, const char *message) {
    if (server->onMessageReceived != NULL) {
        //server->onMessageReceived(message);
    }
}