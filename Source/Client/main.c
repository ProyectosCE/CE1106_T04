#include "comunicaciones/comServer.h"
#include "comunicaciones/socketServer.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void *communicationLoop(void *arg);

int main() {
    pthread_t communicationThread;

    // Crear hilo para manejar las comunicaciones con el servidor
    if (pthread_create(&communicationThread, NULL, communicationLoop, NULL) != 0) {
        printf("Error al crear el hilo de comunicación\n");
        return 1;
    }

    // Simular envío de mensajes desde main
    while (1) {
        char message[256];
        printf("Ingrese un mensaje para enviar al servidor: ");
        fgets(message, 256, stdin);

        // Remover salto de línea
        message[strcspn(message, "\n")] = '\0';

        sendMessageToServer(message);

        sleep(2);
    }

    pthread_join(communicationThread, NULL);

    return 0;
}

void *communicationLoop(void *arg) {
    startComServer();

    char buffer[1024];

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
