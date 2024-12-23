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


// BIBLIOTECAS EXTERNAS
#include <stdlib.h>
#include <unistd.h>
#include <log.h>

// BIBLIOTECAS DE PROYECTO
#include "comServer.h"
#include "../logs/saveLog.h"

// Puntero estático para almacenar la única instancia de ComServer
static ComServer *comserver_instance = NULL;

/* Function: ComServer_create
   Descripción:
     Crea e inicializa una instancia única del servidor de comunicaciones (`ComServer`). Configura sus
     componentes principales como el servidor de sockets y el procesador de JSON. Si ya existe una
     instancia, retorna la existente.

   Params:
     (Ninguno)

   Returns:
     - ComServer*: Un puntero a la instancia del servidor de comunicaciones creada.
       Retorna `NULL` si ocurre un error durante la inicialización.

   Restriction:
     - Debe garantizarse que `ComServer_destroy` se invoque correctamente para liberar la memoria asignada.
     - El servidor de sockets (`SocketServer_create`) y el procesador de JSON (`JsonProcessor_create`)
       deben estar implementados correctamente.

   Example:
     ComServer *server = ComServer_create();
     if (server == NULL) {
         printf("Error al inicializar el servidor de comunicaciones.\n");
     }

   Problems:
     - Problema: Si no hay suficiente memoria para crear `ComServer`, retorna `NULL`.
       - Solución: Manejar errores y registrar mensajes con `savelog_error`.
     - Problema: Si falla la creación del servidor de sockets o el procesador de JSON, no se inicializa.
       - Solución: Liberar recursos con `ComServer_destroy` y retornar `NULL`.

   References:
     - Ninguna referencia externa específica.
*/
ComServer *ComServer_create() {
    if (comserver_instance != NULL) {
        return comserver_instance;
    }

    comserver_instance = (ComServer *)malloc(sizeof(ComServer));
    if (comserver_instance == NULL) {
        savelog_error("Error al crear el servidor de comunicaciones.\n");
        return NULL;
    }

    comserver_instance->socketServer = SocketServer_create();
    comserver_instance->jsonProcessor = JsonProcessor_create();
    comserver_instance->onMessageReceived = NULL;  // Callback inicializado a NULL

    if (comserver_instance->socketServer == NULL || comserver_instance->jsonProcessor == NULL) {
        ComServer_destroy(comserver_instance);
        return NULL;
    }

    SocketServer_start(comserver_instance->socketServer);

    return comserver_instance;
}

/* Function: ComServer_destroy
   Descripción:
     Libera todos los recursos asociados al servidor de comunicaciones (`ComServer`), incluyendo
     el servidor de sockets y el procesador de JSON. Establece la instancia global a `NULL` después
     de liberar la memoria.

   Params:
     server - Puntero al servidor de comunicaciones (`ComServer *`) que se destruirá.

   Returns:
     - void: No retorna valores.

   Restriction:
     - El puntero `server` debe ser válido y no `NULL` al momento de llamar a esta función.
     - Debe invocarse después de `ComServer_create` para liberar recursos correctamente.

   Example:
     ComServer_destroy(server);
     // Libera los recursos del servidor y establece la instancia global a NULL.

   Problems:
     - Problema: Si `server` ya fue liberado previamente, se podría intentar liberar memoria inválida.
       - Solución: Validar que `server` no sea `NULL` antes de proceder.

   References:
     - Ninguna referencia externa específica.
*/
void ComServer_destroy(ComServer *server) {
    if (server != NULL) {
        SocketServer_destroy(server->socketServer);
        JsonProcessor_destroy(server->jsonProcessor);
        free(server);
        comserver_instance = NULL;
    }
}

/* Function: ComServer_sendMessage
   Descripción:
     Envía un mensaje al servidor de comunicaciones. Convierte el mensaje en un formato JSON utilizando
     el procesador de JSON antes de enviarlo a través del servidor de sockets.

   Params:
     server - Puntero al servidor de comunicaciones (`ComServer *`) utilizado para enviar el mensaje.
     message - Cadena de texto que contiene el mensaje a enviar.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `server` debe estar correctamente inicializado mediante `ComServer_create`.
     - El mensaje (`message`) debe ser una cadena válida y no nula.

   Example:
     ComServer_sendMessage(server, "Mensaje de prueba");
     // Envía un mensaje al servidor a través del servidor de sockets.

   Problems:
     - Problema: Si `server` es `NULL`, no se podrá enviar el mensaje.
       - Solución: Registrar una advertencia con `savelog_warn` y evitar operaciones adicionales.
     - Problema: Si `JsonProcessor_createJsonMessage` o `SocketServer_send` fallan, el mensaje no se enviará.
       - Solución: Manejar errores apropiadamente y registrar los mensajes.

   References:
     - Ninguna referencia externa específica.
*/
void ComServer_sendMessage(ComServer *server, const char *message) {
    if (server == NULL) {
        savelog_warn("Servidor no inicializado.\n");
        return;
    }

    char *jsonMessage = JsonProcessor_createJsonMessage(server->jsonProcessor, message);
    savelog_info(jsonMessage);
    SocketServer_send(server->socketServer, jsonMessage);
    free(jsonMessage);
}

/* Function: ComServer_sendPlayerName
   Descripción:
     Envía el nombre del jugador al servidor de comunicaciones. El nombre se convierte a formato JSON
     utilizando el procesador de JSON antes de ser enviado.

   Params:
     server - Puntero al servidor de comunicaciones (`ComServer *`) utilizado para enviar el mensaje.
     name - Cadena de texto que contiene el nombre del jugador.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `server` debe estar correctamente inicializado mediante `ComServer_create`.
     - `name` debe ser una cadena válida y no nula.

   Example:
     ComServer_sendPlayerName(server, "Jugador1");
     // Envía el nombre del jugador al servidor.

   Problems:
     - Problema: Si `server` es `NULL`, no se puede enviar el nombre.
       - Solución: Registrar una advertencia con `savelog_warn` y evitar operaciones adicionales.
     - Problema: Si `JsonProcessor_createJsonPlayerName` falla, no se enviará el mensaje.
       - Solución: Manejar errores y registrar mensajes de fallo.

   References:
     - Ninguna referencia externa específica.
*/
void ComServer_sendPlayerName(ComServer *server, const char *name) {
    if (server == NULL) {
        savelog_warn("Servidor no inicializado.\n");
        return;
    }

    char *jsonMessage = JsonProcessor_createJsonPlayerName(server->jsonProcessor, name);
    SocketServer_send(server->socketServer, jsonMessage);
    free(jsonMessage);
}

/* Function: ComServer_observerGetlist
   Descripción:
     Envía una solicitud al servidor de comunicaciones para obtener la lista de jugadores. La solicitud
     se convierte a formato JSON utilizando el procesador de JSON antes de ser enviada.

   Params:
     server - Puntero al servidor de comunicaciones (`ComServer *`) utilizado para enviar la solicitud.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `server` debe estar correctamente inicializado mediante `ComServer_create`.

   Example:
     ComServer_observerGetlist(server);
     // Envía una solicitud al servidor para obtener la lista de jugadores.

   Problems:
     - Problema: Si `server` es `NULL`, no se puede enviar la solicitud.
       - Solución: Registrar una advertencia con `savelog_warn` y evitar operaciones adicionales.
     - Problema: Si `JsonProcessor_createJsonGetListPlayers` falla, no se enviará la solicitud.
       - Solución: Manejar errores y registrar mensajes de fallo.

   References:
     - Ninguna referencia externa específica.
*/
void ComServer_observerGetlist(ComServer *server) {
    if (server == NULL) {
        savelog_warn("Servidor no inicializado.\n");
        return;
    }

    char *jsonMessage = JsonProcessor_createJsonGetListPlayers(server->jsonProcessor);
    SocketServer_send(server->socketServer, jsonMessage);
    free(jsonMessage);
}


/* Function: comServer_sendChoosenPlayer
   Descripción:
     Envía al servidor de comunicaciones el ID del jugador elegido por el observador en formato JSON.

   Params:
     server - Puntero al servidor de comunicaciones (`ComServer *`) utilizado para enviar el mensaje.
     playerId - Cadena de texto que contiene el ID del jugador elegido.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `server` debe estar correctamente inicializado mediante `ComServer_create`.
     - `playerId` debe ser una cadena válida y no nula.

   Example:
     comServer_sendChoosenPlayer(server, "12345");
     // Envía el ID del jugador elegido al servidor.

   Problems:
     - Problema: Si `server` es `NULL`, no se puede enviar el ID del jugador.
       - Solución: Registrar una advertencia con `savelog_warn` y evitar operaciones adicionales.
     - Problema: Si `JsonProcessor_createJsonChoosenPlayer` falla, no se enviará el mensaje.
       - Solución: Manejar errores y registrar mensajes de fallo.

   References:
     - Ninguna referencia externa específica.
*/
void comServer_sendChoosenPlayer(ComServer *server,const char *playerId) {
        if (server == NULL) {
            savelog_warn("Servidor no inicializado.\n");
            return;
        }

        char *jsonMessage = JsonProcessor_createJsonChoosenPlayer(server->jsonProcessor,playerId);
        SocketServer_send(server->socketServer, jsonMessage);
        free(jsonMessage);

}

/* Function: ComServer_sendStatus
   Descripción:
     Envía un mensaje de estado al servidor de comunicaciones a través del servidor de sockets.

   Params:
     message - Cadena de texto que contiene el mensaje de estado a enviar.

   Returns:
     - void: No retorna valores.

   Restriction:
     - El servidor debe estar correctamente inicializado mediante `ComServer_create`.
     - `message` debe ser una cadena válida y no nula.

   Example:
     ComServer_sendStatus("Estado actualizado");
     // Envía un mensaje de estado al servidor.

   Problems:
     - Problema: Si `comserver_instance` es `NULL`, no se puede enviar el mensaje.
       - Solución: Registrar una advertencia con `savelog_warn` y evitar operaciones adicionales.
     - Problema: Si `message` es nulo o inválido, puede ocurrir un error en `SocketServer_send`.
       - Solución: Validar `message` antes de procesarlo.

   References:
     - Ninguna referencia externa específica.
*/


void ComServer_sendStatus(const char *message) {
    if (comserver_instance == NULL) {
        savelog_warn("Servidor no inicializado.\n");
        return;
    }
    SocketServer_send(comserver_instance->socketServer, message);
    free(message);
}

/* Function: ComServer_registerCallback
   Descripción:
     Registra un callback para manejar mensajes recibidos por el servidor de comunicaciones.

   Params:
     server - Puntero al servidor de comunicaciones (`ComServer *`) donde se registrará el callback.
     callback - Puntero a la función que se ejecutará al recibir un mensaje (observer).

   Returns:
     - void: No retorna valores.

   Restriction:
     - `server` debe estar correctamente inicializado mediante `ComServer_create`.

   Example:
     void myCallback(const char *message) {
         printf("Mensaje recibido: %s\n", message);
     }
     ComServer_registerCallback(server, myCallback);
     // Registra un callback para manejar mensajes recibidos.

   Problems:
     - Problema: Si `server` es `NULL`, no se puede registrar el callback.
       - Solución: Manejar el caso registrando un mensaje de error o advertencia.

   References:
     - Ninguna referencia externa específica.
*/
void ComServer_registerCallback(ComServer *server, MessageReceivedCallback callback) {
    if (server != NULL) {
        printf("Se registro el callback corectamente\n");
        server->onMessageReceived = callback;
    }
    else {
        printf("UPS, no se puedo registrar el callback");
    }
}

/* Function: ComServer_messageListeningLoop
   Descripción:
     Ejecuta un bucle continuo para recibir mensajes del servidor a través del socket. Cuando se recibe un
     mensaje, lo pasa al callback registrado (`onMessageReceived`) si está definido.

   Params:
     arg - Puntero a la instancia del servidor de comunicaciones (`ComServer *`).

   Returns:
     - void*: Retorna `NULL` en caso de error durante la inicialización del servidor.

   Restriction:
     - `arg` debe ser un puntero válido a una instancia inicializada de `ComServer`.
     - El callback `onMessageReceived` debe estar configurado si se desea procesar los mensajes.

   Example:
     pthread_t listenerThread;
     pthread_create(&listenerThread, NULL, ComServer_messageListeningLoop, (void *)server);
     // Inicia un hilo para escuchar mensajes del servidor.

   Problems:
     - Problema: Si `arg` es `NULL`, no se puede inicializar el bucle de escucha.
       - Solución: Registrar un mensaje de error con `savelog_error` y retornar `NULL`.
     - Problema: Si el servidor pierde la conexión, puede entrar en un estado de reconexión continua.
       - Solución: Limitar los intentos de reconexión o implementar un tiempo de espera.

   References:
     - Ninguna referencia externa específica.
*/
void *ComServer_messageListeningLoop(void *arg) {
    ComServer *server = (ComServer *)arg;
    if (server == NULL) {
        savelog_error("Servidor no inicializado para la escucha de mensajes.\n");
        return NULL;
    }

    char buffer[4096];
    while (1) {
        if (server->socketServer->isConnected) {
            int bytesReceived = SocketServer_receive(server->socketServer, buffer, sizeof(buffer));
            if (bytesReceived > 0) {
                if (server->onMessageReceived != NULL) {
                    server->onMessageReceived(buffer);  // Notificar al observer
                }
                //free(processedMessage);
            } else {
                log_error("Error al recibir el mensaje del servidor\n");
            }
        } else {
            SocketServer_reconnect(server->socketServer);
        }
    }
}

/* Function: ComServer_handleReceivedMessage
Descripción:
Maneja mensajes recibidos llamando al callback registrado (`onMessageReceived`), si está configurado.

Params:
server - Puntero a la instancia del servidor de comunicaciones (`ComServer *`).
message - Cadena de texto que contiene el mensaje recibido.

Returns:
- void: No retorna valores.

Restriction:
- `server` debe estar correctamente inicializado.
- El callback `onMessageReceived` debe estar configurado si se desea procesar mensajes.

Example:
ComServer_handleReceivedMessage(server, "Mensaje de prueba");
// Maneja el mensaje recibido invocando el callback configurado.

Problems:
- Problema: Si `server` o `message` son `NULL`, el mensaje no será procesado.
- Solución: Validar ambos parámetros antes de proceder.

References:
- Ninguna referencia externa específica.
*/
void ComServer_handleReceivedMessage(ComServer *server, const char *message) {
    if (server->onMessageReceived != NULL) {
        //server->onMessageReceived(message);
    }
}