#include "comServer.h"
#include "socketServer.h"
#include "jsonProcessor.h"
#include <stdio.h>
#include <malloc.h>

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

    // Enviar el mensaje procesado a main para que lo use
    printf("Mensaje recibido del servidor y procesado: %s\n", processedMessage);

    // Liberar memoria
    free(processedMessage);
}
