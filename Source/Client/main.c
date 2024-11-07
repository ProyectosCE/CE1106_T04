#include "comunicaciones/comServer.h"
#include "comunicaciones/jsonProcessor.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

ComServer *crearYConectarComServer(const char *clientType) {
    // Crear la instancia de ComServer con el tipo de cliente seleccionado
    ComServer *server = ComServer_create(clientType);

    // Crear y enviar el mensaje inicial de tipo de cliente usando JsonProcessor
    char *initMessage = JsonProcessor_createClientTypeMessage(clientType);
    if (initMessage != NULL) {
        ComServer_sendMessage(server, initMessage);
        free(initMessage);
    } else {
        printf("Error al crear el mensaje de tipo de cliente.\n");
        ComServer_destroy(server);
        return NULL;
    }

    return server;
}

int main() {
    int seleccion;
    const char *clientType;

    // Pedir al usuario que seleccione el tipo de cliente
    printf("Seleccione el modo:\n1. Jugador\n2. Espectador\n");
    scanf("%d", &seleccion);

    // Determinar el tipo de cliente basado en la selección
    if (seleccion == 1) {
        clientType = "player";
    } else if (seleccion == 2) {
        clientType = "spectator";
    } else {
        printf("Selección no válida. Saliendo...\n");
        return 1;
    }

    // Crear y conectar ComServer con el tipo de cliente seleccionado
    ComServer *server = crearYConectarComServer(clientType);
    if (server == NULL) {
        printf("No se pudo conectar al servidor. Saliendo...\n");
        return 1;
    }

    // Aquí puede añadirse el bucle de juego o el código para recibir mensajes
    sleep(10);

    // Al final, destruir el servidor de comunicaciones
    ComServer_destroy(server);

    return 0;
}
