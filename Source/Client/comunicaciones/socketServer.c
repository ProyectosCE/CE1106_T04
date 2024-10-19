#include "socketServer.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345

/*
 * Constructor: Inicializa la estructura SocketServer y prepara la conexión
 */
SocketServer *SocketServer_create() {
    SocketServer *server = (SocketServer *)malloc(sizeof(SocketServer));
    if (server == NULL) {
        printf("Error al asignar memoria para SocketServer\n");
        return NULL;
    }

    server->sock = 0;
    memset(&(server->serverAddress), 0, sizeof(server->serverAddress));

    return server;
}

/*
 * Destructor: Libera los recursos de SocketServer
 */
void SocketServer_destroy(SocketServer *server) {
    if (server != NULL) {
        if (server->sock > 0) {
            close(server->sock);  // Cerrar el socket si está abierto
        }
        free(server);  // Liberar la memoria de la estructura
    }
}

/*
 * Método para iniciar la conexión del socket con el servidor
 */
void SocketServer_start(SocketServer *server) {
    if ((server->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error al crear el socket\n");
        return;
    }

    server->serverAddress.sin_family = AF_INET;
    server->serverAddress.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server->serverAddress.sin_addr) <= 0) {
        printf("Dirección IP inválida\n");
        return;
    }

    // Intentar conectar al servidor
    while (connect(server->sock, (struct sockaddr *)&(server->serverAddress), sizeof(server->serverAddress)) < 0) {
        printf("Error al conectar al servidor, reintentando en 1 segundo...\n");
        sleep(1);
    }

    printf("Conectado al servidor\n");
}

/*
 * Método para enviar un mensaje al servidor
 */
void SocketServer_send(SocketServer *server, const char *message) {
    if (send(server->sock, message, strlen(message), 0) < 0) {
        printf("Error al enviar el mensaje\n");
    } else {
        printf("Mensaje enviado al servidor: %s\n", message);
    }
}

/*
 * Método para recibir un mensaje del servidor
 */
int SocketServer_receive(SocketServer *server, char *buffer, int bufferSize) {
    int valread = read(server->sock, buffer, bufferSize - 1);
    if (valread > 0) {
        buffer[valread] = '\0';  // Añadir terminador de cadena al buffer
        printf("Mensaje recibido del servidor: %s\n", buffer);
    } else {
        printf("Error al recibir el mensaje del servidor\n");
    }
    return valread;
}
