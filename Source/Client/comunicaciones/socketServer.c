#include "socketServer.h"
#include "../configuracion/configuracion.h"
#include "../logs/saveLog.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <log.h>

// Puntero estático para almacenar la única instancia de SocketServer
static SocketServer *socketServer_instance = NULL;

/*
 * Constructor: Inicializa la estructura SocketServer y prepara la conexión
 */
SocketServer *SocketServer_create() {
    if (socketServer_instance != NULL) {
        return socketServer_instance;
    }

    socketServer_instance = (SocketServer *)malloc(sizeof(SocketServer));
    if (socketServer_instance == NULL) {
        log_fatal("Error al asignar memoria para SocketServer\n");
        return NULL;
    }

    const char* address = get_config_string("socket.address");
    socketServer_instance->ipServidor = address;  // Apuntando directamente
    log_debug("IP del servidor: %s\n", socketServer_instance->ipServidor);
    socketServer_instance->port= get_config_int("socket.port");
    socketServer_instance->sock = 0;
    socketServer_instance->isConnected = 0;  // El servidor comienza como desconectado
    memset(&(socketServer_instance->serverAddress), 0, sizeof(socketServer_instance->serverAddress));

    return socketServer_instance;
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
        socketServer_instance = NULL;
    }
}

/*
 * Método para configurar la dirección del servidor
 */
void SocketServer_configureAddress(SocketServer *server) {
    server->serverAddress.sin_family = AF_INET;
    server->serverAddress.sin_port = htons(server->port);

    if (inet_pton(AF_INET, server->ipServidor, &server->serverAddress.sin_addr) <= 0) {
        log_fatal("Dirección IP inválida\n");
    }
}

/*
 * Método para iniciar la conexión del socket con el servidor
 */
void SocketServer_start(SocketServer *server) {
    if ((server->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        log_fatal("Error al crear el socket\n");
        return;
    }

    SocketServer_configureAddress(server);  // Configurar la dirección del servidor

    // Intentar conectar al servidor
    while (connect(server->sock, (struct sockaddr *)&(server->serverAddress), sizeof(server->serverAddress)) < 0) {
        savelog_error("Error al conectar al servidor, reintentando en 1 segundo...\n");
        sleep(1);
    }

    log_info("Conectado al servidor en el puerto %d\n", ntohs(server->serverAddress.sin_port));
    server->isConnected = 1;  // Marcar como conectado
}

/*
 * Método para enviar un mensaje al servidor
 */

void SocketServer_send(SocketServer *server, const char *message) {
    if (!server->isConnected) {
        log_warn("El servidor no está disponible, no se puede enviar el mensaje.\n");
        return;
    }

    // Concatenar el mensaje con un delimitador de fin de línea.
    char messageWithDelimiter[4096];  // Ajusta el tamaño si es necesario
    snprintf(messageWithDelimiter, sizeof(messageWithDelimiter), "%s\n", message);

    size_t totalBytesSent = 0;
    size_t messageLength = strlen(messageWithDelimiter);

    // Bucle para enviar todos los datos
    while (totalBytesSent < messageLength) {
        ssize_t bytesSent = send(server->sock, messageWithDelimiter + totalBytesSent, messageLength - totalBytesSent, 0);

        if (bytesSent < 0) {
            log_error("Error al enviar el mensaje\n");
            return;
        }

        totalBytesSent += bytesSent;
    }

    //log_info("Mensaje enviado al servidor: %s\n", message);
}

/*
 * Método para recibir un mensaje del servidor con manejo de desconexión
 */
int SocketServer_receive(SocketServer *server, char *buffer, int bufferSize) {
    if (!server->isConnected) {
        log_warn("El servidor no está disponible, no se puede recibir mensajes.\n");
        return -1;  // No recibir mensajes si el servidor está desconectado
    }

    int valread = read(server->sock, buffer, bufferSize - 1);

    if (valread == 0) {
        log_warn("El servidor se ha desconectado\n");
        server->isConnected = 0;  // Marcar como desconectado
        return 0;  // Indicar que la conexión se ha cerrado
    } else if (valread < 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            return -1;  // Indicar que no hay datos sin error crítico
        } else {
            server->isConnected = 0;  // Marcar como desconectado en caso de error
            return -1;  // Indicar error
        }
    } else {
        buffer[valread] = '\0';  // Añadir terminador de cadena al buffer
        //log_info("Mensaje recibido del servidor: %s\n", buffer);
        return valread;  // Retornar la cantidad de bytes leídos
    }
}

/*
 * Método para reconectar si el servidor está desconectado
 */
void SocketServer_reconnect(SocketServer *server) {
    if (!server->isConnected) {
        log_warn("Intentando reconectar al servidor...\n");
        SocketServer_start(server);  // Intentar reconectar
    }
}