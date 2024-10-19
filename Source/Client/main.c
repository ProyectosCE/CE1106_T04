#include "comunicaciones/comServer.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

int main() {
    pthread_t communicationThread;

    // Iniciar el servidor de comunicaciones (establecer conexión de socket)
    startComServer();  // Llama a startComServer() para iniciar la conexión de socket

    // Crear el hilo para manejar las comunicaciones con el servidor
    if (pthread_create(&communicationThread, NULL, messageListeningLoop, NULL) != 0) {
        printf("Error al crear el hilo de comunicación\n");
        return 1;
    }

    // Simular envío de mensajes desde main
    while (1) {
        char message[256];
        printf("Ingrese un mensaje para enviar al servidor: ");
        fgets(message, 256, stdin);

        // Remover el salto de línea (newline) del mensaje de entrada
        message[strcspn(message, "\n")] = '\0';

        // Enviar el mensaje al servidor
        sendMessageToServer(message);

        // Recibir mensajes procesados desde comServer
        char *response = getProcessedMessage();
        if (response != NULL) {
            printf("Respuesta procesada del servidor: %s\n", response);
            free(response);  // Liberar memoria de la respuesta procesada
        }

        sleep(2);  // Pausa de 2 segundos antes de enviar el siguiente mensaje
    }

    pthread_join(communicationThread, NULL);

    return 0;
}
