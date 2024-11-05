#include "comServer.h"
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
    // Verifica si ya existe una instancia
    if (comserver_instance != NULL) {
        return comserver_instance;
    }

    // Crear nueva instancia si no existe
    comserver_instance = (ComServer *)malloc(sizeof(ComServer));
    if (comserver_instance == NULL) {
        savelog_error("Error al crear el servidor de comunicaciones.\n");
        return NULL;
    }

    // Inicializar los componentes
    comserver_instance->socketServer = SocketServer_create();
    comserver_instance->jsonProcessor = JsonProcessor_create();
    comserver_instance->onMessageReceived = NULL;  // Callback inicializado a NULL

    if (comserver_instance->socketServer == NULL || comserver_instance->jsonProcessor == NULL) {
        ComServer_destroy(comserver_instance);  // Liberar recursos si algo falla
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

    // Convertir el mensaje a JSON utilizando JsonProcessor
    char *jsonMessage = JsonProcessor_createJsonMessage(server->jsonProcessor, message);

    savelog_info(jsonMessage);


    // Enviar el mensaje al servidor usando el SocketServer
    SocketServer_send(server->socketServer, jsonMessage);

    // Liberar memoria
    free(jsonMessage);
}

/*
 * Método para registrar un callback (Observer) para recibir notificaciones
 */
void ComServer_registerCallback(ComServer *server, MessageReceivedCallback callback) {
    if (server != NULL) {
        server->onMessageReceived = callback;
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

    char buffer[1024];
    //SocketServer_isConnected(server->socketServer);
    // Loop de recepción de mensajes
    while (1) {

        if (server->socketServer->isConnected) {
            int bytesReceived = SocketServer_receive(server->socketServer, buffer, sizeof(buffer));
            if (bytesReceived > 0) {
                // Procesar el mensaje recibido
                char *processedMessage = JsonProcessor_processJsonMessage(server->jsonProcessor, buffer);

                // Si el callback está registrado, notificar a main
                if (server->onMessageReceived != NULL) {
                    server->onMessageReceived(processedMessage);  // Notificar al observer
                }

                free(processedMessage);  // Liberar memoria del mensaje procesado
            } else {
                savelog_error("Error al recibir el mensaje del servidor\n");
            }
            sleep(2);  // Pausar un poco antes de recibir el próximo mensaje
        } else{
            SocketServer_reconnect(server->socketServer);
        }
    }
}
