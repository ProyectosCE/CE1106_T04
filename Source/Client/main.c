#include "comunicaciones/comServer.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// Callback para manejar la recepción de mensajes desde ComServer
void onMessageReceived(const char *message) {
    printf("Respuesta procesada del servidor: %s\n", message);
}

int main() {
    pthread_t communicationThread;

    // Crear la instancia del ComServer
    ComServer *comServer = ComServer_create();

    // Registrar el callback para recibir mensajes procesados desde el servidor
    ComServer_registerCallback(comServer, onMessageReceived);


    // Crear el hilo para manejar las comunicaciones con el servidor
    if (pthread_create(&communicationThread, NULL, ComServer_messageListeningLoop, (void *)comServer) != 0) {
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
        ComServer_sendMessage(comServer, message);



        sleep(2);  // Pausa de 2 segundos antes de enviar el siguiente mensaje
    }

    pthread_join(communicationThread, NULL);

    // Destruir el servidor de comunicaciones al final
    ComServer_destroy(comServer);

    return 0;
}

