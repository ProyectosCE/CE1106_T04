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
 * Constructor: Inicializa la estructura SocketServer sin conectarse al servidor
 */
SocketServer *SocketServer_create() {
    // Verifica si ya existe una instancia
    if (socketServer_instance != NULL) {
        return socketServer_instance;
    }

    socketServer_instance = (SocketServer *)malloc(sizeof(SocketServer));
    if (socketServer_instance == NULL) {
        log_fatal("Error al asignar memoria para SocketServer\n");
        return NULL;
    }

    // Configurar los valores iniciales
    Configuracion *configuracion = crear_configuracion();
    strcpy(socketServer_instance->ipServidor, leer_ip(configuracion));
    socketServer_instance->port = leer_puerto(configuracion);
    socketServer_instance->sock = 0;
    socketServer_instance->isConnected = 0;  // El servidor comienza como desconectado
    memset(&(socketServer_instance->serverAddress), 0, sizeof(socketServer_instance->serverAddress));
    free(configuracion);

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

// Devuelve el estado de la conexión
bool SocketServer_isConnected(SocketServer *server) {
    return server->isConnected;
}

/*
 * Conectar el socket al servidor (se llama explícitamente desde el menú)
 */
void SocketServer_connect(SocketServer *server) {
    if ((server->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        log_fatal("Error al crear el socket\n");
        return;
    }

    server->serverAddress.sin_family = AF_INET;
    server->serverAddress.sin_port = htons(server->port);

    if (inet_pton(AF_INET, server->ipServidor, &server->serverAddress.sin_addr) <= 0) {
        log_fatal("Dirección IP inválida\n");
        return;
    }

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
        return;  // No enviar el mensaje si el servidor está desconectado
    }

    if (send(server->sock, message, strlen(message), 0) < 0) {
        log_error("Error al enviar el mensaje\n");
    } else {
        log_info("Mensaje enviado al servidor: %s\n", message);
    }
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
        log_info("Mensaje recibido del servidor: %s\n", buffer);
        return valread;  // Retornar la cantidad de bytes leídos
    }
}

/*
 * Reconectar si el servidor está desconectado
 */
void SocketServer_reconnect(SocketServer *server) {
    if (!server->isConnected) {
        log_warn("Intentando reconectar al servidor...\n");
        SocketServer_connect(server);  // Intentar reconectar
    }
}
