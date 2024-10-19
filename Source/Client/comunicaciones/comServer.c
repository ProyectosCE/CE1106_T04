#include "comServer.h"
#include "socketServer.h"
#include "jsonProcessor.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>

char *lastProcessedMessage = NULL;  // Variable para almacenar el último mensaje procesado

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

    // Si ya había un mensaje procesado previo, liberarlo
    if (lastProcessedMessage != NULL) {
        free(lastProcessedMessage);
    }

    // Almacenar el nuevo mensaje procesado
    lastProcessedMessage = strdup(processedMessage);

    // Liberar memoria
    free(processedMessage);
}

/*
 * Obtener el último mensaje procesado para el main
 */
char *getProcessedMessage() {
    // Retornar una copia del último mensaje procesado
    if (lastProcessedMessage != NULL) {
        return strdup(lastProcessedMessage);  // Retornar una copia para que main lo maneje
    }
    return NULL;  // No hay mensaje disponible
}

/*
 * Bucle que maneja la escucha de mensajes entrantes del servidor
 */
_Noreturn void *messageListeningLoop(void *arg) {
    char buffer[1024];
    // Bucle infinito para recibir mensajes del servidor
    while (1) {
        int bytesReceived = receiveFromServer(buffer);  // Recibir mensaje del servidor
        if (bytesReceived > 0) {
            // Procesar el mensaje recibido
            processIncomingMessage(buffer);
        } else {
            printf("Error al recibir el mensaje del servidor\n");
        }
        sleep(2);  // Pausa antes de la próxima iteración
    }
}
