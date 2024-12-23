/*
================================== LICENCIA ==================================================
MIT License
Copyright (c) 2024 José Bernardo Barquero Bonilla,
                   Jose Eduardo Campos Salazar,
                   Jimmy Feng Feng,
                   Alexander Montero Vargas
Consulta el archivo LICENSE para más detalles.
==============================================================================================
*/



// BIBLIOTECAS DE PROYECTO
#include "socketServer.h"
#include "../configuracion/configuracion.h"
#include "../logs/saveLog.h"

// BIBLIOTECAS DE EXTERNAS
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <log.h>

// Puntero estático para almacenar la única instancia de SocketServer
static SocketServer *socketServer_instance = NULL;

/* Function: SocketServer_create
   Descripción:
     Crea e inicializa una instancia única de `SocketServer`, configurando los parámetros de conexión
     a partir de valores de configuración. Establece la IP del servidor, el puerto y otros valores iniciales.

   Params:
     (Ninguno)

   Returns:
     - SocketServer*: Un puntero a la instancia creada del servidor de sockets.
       Retorna `NULL` si ocurre un error durante la inicialización.

   Restriction:
     - La configuración del socket (IP y puerto) debe estar correctamente definida y accesible
       a través de las funciones `get_config_string` y `get_config_int`.
     - `SocketServer_destroy` debe ser llamado para liberar los recursos asignados.

   Example:
     SocketServer *server = SocketServer_create();
     if (server == NULL) {
         printf("Error al inicializar el servidor de sockets.\n");
     }

   Problems:
     - Problema: Si no hay suficiente memoria para crear `SocketServer`, retorna `NULL`.
       - Solución: Registrar un mensaje crítico con `log_fatal`.
     - Problema: Si no se pueden obtener los valores de configuración para la IP o el puerto,
       la configuración del socket será incompleta.
       - Solución: Validar que los valores devueltos por `get_config_string` y `get_config_int` sean válidos.

   References:
     - Ninguna referencia externa específica.
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

/* Function: SocketServer_destroy
   Descripción:
     Libera todos los recursos asociados al `SocketServer`, incluyendo el cierre del socket si está abierto,
     y libera la memoria de la instancia.

   Params:
     server - Puntero a la instancia del servidor de sockets (`SocketServer *`) a liberar.

   Returns:
     - void: No retorna valores.

   Restriction:
     - El puntero `server` debe ser válido y no `NULL` al momento de llamar a esta función.
     - La función debe ser llamada después de `SocketServer_create` para liberar correctamente los recursos asignados.

   Example:
     SocketServer_destroy(server);
     // Libera los recursos del servidor y elimina la instancia global.

   Problems:
     - Problema: Si `server` ya fue liberado previamente, puede causar un fallo.
       - Solución: Validar que `server` no sea `NULL` antes de proceder.

   References:
     - Ninguna referencia externa específica.
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

/* Function: SocketServer_configureAddress
   Descripción:
     Configura la estructura de dirección del servidor (`serverAddress`) utilizando la dirección IP
     y el puerto definidos en la instancia del servidor de sockets.

   Params:
     server - Puntero a la instancia del servidor de sockets (`SocketServer *`) a configurar.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `server` debe estar correctamente inicializado.
     - La dirección IP (`ipServidor`) debe ser válida para `AF_INET`.

   Example:
     SocketServer_configureAddress(server);
     // Configura la dirección del servidor.

   Problems:
     - Problema: Si la dirección IP no es válida, el programa registrará un error crítico.
       - Solución: Validar previamente la IP antes de configurar la dirección.

   References:
     - Ninguna referencia externa específica.
*/
void SocketServer_configureAddress(SocketServer *server) {
    server->serverAddress.sin_family = AF_INET;
    server->serverAddress.sin_port = htons(server->port);

    if (inet_pton(AF_INET, server->ipServidor, &server->serverAddress.sin_addr) <= 0) {
        log_fatal("Dirección IP inválida\n");
    }
}

/* Function: SocketServer_start
   Descripción:
     Crea un socket e intenta conectarse al servidor definido en la instancia del `SocketServer`.
     Reintenta la conexión en caso de fallo hasta que se establezca exitosamente.

   Params:
     server - Puntero a la instancia del servidor de sockets (`SocketServer *`) que iniciará la conexión.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `server` debe estar correctamente inicializado y configurado con `SocketServer_configureAddress`.
     - La dirección IP y el puerto deben ser válidos.

   Example:
     SocketServer_start(server);
     // Crea un socket y conecta al servidor.

   Problems:
     - Problema: Si el socket no puede ser creado, se registrará un error crítico y no se establecerá conexión.
       - Solución: Validar la creación del socket antes de proceder.
     - Problema: Si la conexión falla repetidamente, puede causar bloqueos en el programa.
       - Solución: Implementar un límite de reintentos o manejar adecuadamente el tiempo de espera.

   References:
     - Ninguna referencia externa específica.
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

/* Function: SocketServer_send
   Descripción:
     Envía un mensaje al servidor a través del socket. Si el servidor no está conectado, registra una advertencia.
     El mensaje se concatena con un delimitador de fin de línea antes de ser enviado.

   Params:
     server - Puntero a la instancia del servidor de sockets (`SocketServer *`) que envía el mensaje.
     message - Cadena de texto que contiene el mensaje a enviar.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `server` debe estar correctamente inicializado y conectado al servidor (`isConnected` debe ser `true`).
     - `message` debe ser una cadena válida y no nula.

   Example:
     SocketServer_send(server, "Hola, servidor");
     // Envía un mensaje al servidor conectado.

   Problems:
     - Problema: Si el servidor no está conectado, el mensaje no se enviará.
       - Solución: Validar el estado de conexión antes de intentar enviar.
     - Problema: Si ocurre un error durante el envío, los datos pueden no ser enviados completamente.
       - Solución: Manejar errores y registrar mensajes para depuración.

   References:
     - Ninguna referencia externa específica.
     */

void SocketServer_send(SocketServer *server, const char *message) {
    if (!server->isConnected) {
        log_warn("El servidor no está disponible, no se puede enviar el mensaje.\n");
        return;
    }

    // Concatenar el mensaje con un delimitador de fin de línea.
    char messageWithDelimiter[8192];  // Ajusta el tamaño si es necesario
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

/* Function: SocketServer_receive
   Descripción:
     Recibe un mensaje del servidor a través del socket y maneja posibles desconexiones.
     Actualiza el estado de conexión del servidor según sea necesario.

   Params:
     server - Puntero a la instancia del servidor de sockets (`SocketServer *`) que recibe el mensaje.
     buffer - Puntero al buffer donde se almacenará el mensaje recibido.
     bufferSize - Tamaño máximo del buffer.

   Returns:
     - int: Cantidad de bytes recibidos si se completa exitosamente.
       Retorna 0 si el servidor se desconecta, o -1 si ocurre un error.

   Restriction:
     - `server` debe estar correctamente inicializado y conectado al servidor (`isConnected` debe ser `true`).
     - `buffer` debe ser un puntero válido y el tamaño (`bufferSize`) suficiente para almacenar el mensaje.

   Example:
     char buffer[1024];
     int bytes = SocketServer_receive(server, buffer, sizeof(buffer));
     if (bytes > 0) {
         printf("Mensaje recibido: %s\n", buffer);
     }

   Problems:
     - Problema: Si el servidor se desconecta, el método retorna 0 y marca el servidor como desconectado.
       - Solución: Manejar el estado de desconexión y reconectar si es necesario.
     - Problema: Si el buffer no es suficientemente grande, se puede truncar el mensaje recibido.
       - Solución: Asegurar que el tamaño del buffer sea adecuado.

   References:
     - Ninguna referencia externa específica.
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

/* Function: SocketServer_reconnect
   Descripción:
     Intenta reconectar al servidor si está desconectado, utilizando la configuración previa.

   Params:
     server - Puntero a la instancia del servidor de sockets (`SocketServer *`) que intentará reconectarse.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `server` debe estar correctamente inicializado.
     - La dirección y el puerto del servidor deben estar configurados correctamente.

   Example:
     SocketServer_reconnect(server);
     // Intenta reconectar al servidor si la conexión actual está cerrada.

   Problems:
     - Problema: Si la conexión falla repetidamente, puede causar un bucle de reconexión continuo.
       - Solución: Implementar un límite de intentos de reconexión o tiempos de espera adecuados.

   References:
     - Ninguna referencia externa específica.
 */
void SocketServer_reconnect(SocketServer *server) {
    if (!server->isConnected) {
        log_warn("Intentando reconectar al servidor...\n");
        SocketServer_start(server);  // Intentar reconectar
    }
}