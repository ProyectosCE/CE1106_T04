#include "socketServer.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

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
    server->isConnected = 0;  // El servidor comienza como desconectado
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

    printf("Conectado al servidor en el puerto %d\n", ntohs(server->serverAddress.sin_port));
    server->isConnected = 1;  // Marcar como conectado

}

/*
 * Método para enviar un mensaje al servidor
 */
void SocketServer_send(SocketServer *server, const char *message) {
    if (!server->isConnected) {
        printf("El servidor no está disponible, no se puede enviar el mensaje.\n");
        return;  // No enviar el mensaje si el servidor está desconectado
    }

    if (send(server->sock, message, strlen(message), 0) < 0) {
        printf("Error al enviar el mensaje\n");
    } else {
        printf("Mensaje enviado al servidor: %s\n", message);
    }
}

/*
 * Método para recibir un mensaje del servidor con manejo de desconexión
 */
int SocketServer_receive(SocketServer *server, char *buffer, int bufferSize) {
    if (!server->isConnected) {
        printf("El servidor no está disponible, no se puede recibir mensajes.\n");
        return -1;  // No recibir mensajes si el servidor está desconectado
    }

    int valread = read(server->sock, buffer, bufferSize - 1);

    if (valread == 0) {
        // Si read devuelve 0, significa que el servidor ha cerrado la conexión
        printf("El servidor se ha desconectado en Receive\n");
        server->isConnected = 0;  // Marcar como desconectado
        return 0;  // Indicar que la conexión se ha cerrado
    } else if (valread < 0) {
        // Si read devuelve -1, ha ocurrido un error, pero puede no ser una desconexión
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            return -1;  // Indicar que no hay datos sin error crítico
        } else {
            server->isConnected = 0;  // Marcar como desconectado en caso de error
            return -1;  // Indicar error
        }
    } else {
        // Se recibieron datos del servidor correctamente
        buffer[valread] = '\0';  // Añadir terminador de cadena al buffer
        printf("Mensaje recibido del servidor: %s\n", buffer);
        return valread;  // Retornar la cantidad de bytes leídos
    }
}


/*
 * Método para reconectar si el servidor está desconectado
 */
void SocketServer_reconnect(SocketServer *server) {
    if (!server->isConnected) {
        printf("Intentando reconectar al servidor...\n");
        SocketServer_start(server);  // Intentar reconectar
    }
}
