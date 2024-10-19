#include "comServer.h"
#include "socketServer.h"
#include "jsonProcessor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char buffer[1024];  // Buffer para recibir mensajes

/*
 * Inicia el servidor de comunicaciones
 */
void startComServer() {
    startSocketConnection();  // Iniciar la conexión de socket
}

/*
 * Recibe un mensaje desde main, lo procesa y lo envía al servidor usando SocketServer
 */
void sendMessageToServer(const char *message) {
    // Convertir mensaje de main a JSON
    char *jsonMessage = processToJson(message);

    // Enviar el mensaje JSON al servidor usando SocketServer
    sendToServer(jsonMessage);

    // Liberar memoria
    free(jsonMessage);
}

/*
 * Procesar mensajes entrantes desde el servidor
 */
void processIncomingMessage(const char *message) {
    // Procesar el JSON recibido
    char *processedMessage = processFromJson(message);

    // Guardar el mensaje procesado para que el main lo use
    printf("Mensaje recibido del servidor y procesado: %s\n", processedMessage);

    // Liberar memoria
    free(processedMessage);
}


/*
 * Espera y procesa los mensajes entrantes desde el servidor
 */
void *messageListeningLoop(void *arg) {
    // Loop para recibir mensajes del servidor
    while (1) {
        int bytesReceived = receiveFromServer(buffer);
        if (bytesReceived > 0) {
            processIncomingMessage(buffer);
        } else {
            printf("Error al recibir el mensaje del servidor\n");
        }
        sleep(2);
    }
    return NULL;
}

/*
 * Recibe un mensaje ya procesado desde el servidor
 */
char *getProcessedMessage() {
    return strdup(buffer);  // Retorna una copia del último mensaje procesado
}
