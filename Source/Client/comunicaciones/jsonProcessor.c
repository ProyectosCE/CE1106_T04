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

#include "jsonProcessor.h"
#include "../logs/saveLog.h"

// BIBLIOTECAS EXTRERNAS
#include <stdlib.h>
#include <string.h>

// Puntero estático para almacenar la única instancia de JsonProcessor
static JsonProcessor *jsonProcessor_instance = NULL;

/* Function: JsonProcessor_create
   Descripción:
     Crea e inicializa una instancia única del procesador JSON (`JsonProcessor`). Si ya existe una instancia,
     retorna la existente.

   Params:
     (Ninguno)

   Returns:
     - JsonProcessor*: Un puntero a la instancia creada del procesador JSON.
       Retorna `NULL` si ocurre un error durante la inicialización.

   Restriction:
     - La instancia debe ser liberada posteriormente con `JsonProcessor_destroy` para evitar fugas de memoria.

   Example:
     JsonProcessor *processor = JsonProcessor_create();
     if (processor == NULL) {
         printf("Error al inicializar el procesador JSON.\n");
     }

   Problems:
     - Problema: Si no hay suficiente memoria para crear `JsonProcessor`, retorna `NULL`.
       - Solución: Manejar errores y registrar un mensaje crítico con `savelog_fatal`.

   References:
     - Ninguna referencia externa específica.
*/
JsonProcessor *JsonProcessor_create() {
    if (jsonProcessor_instance != NULL) {
        return jsonProcessor_instance;
    }

    jsonProcessor_instance = (JsonProcessor *)malloc(sizeof(JsonProcessor));
    if (jsonProcessor_instance == NULL) {
        savelog_fatal("Error al asignar memoria para JsonProcessor\n");
        return NULL;
    }

    return jsonProcessor_instance;
}

/* Function: JsonProcessor_destroy
   Descripción:
     Libera los recursos asociados al procesador JSON (`JsonProcessor`) y elimina la instancia global.

   Params:
     processor - Puntero a la instancia del procesador JSON (`JsonProcessor *`) que se destruirá.

   Returns:
     - void: No retorna valores.

   Restriction:
     - El puntero `processor` debe ser válido y no `NULL` al momento de llamar a esta función.
     - Debe ser invocado después de `JsonProcessor_create` para liberar correctamente los recursos asignados.

   Example:
     JsonProcessor_destroy(processor);
     // Libera los recursos del procesador JSON.

   Problems:
     - Problema: Si `processor` ya fue liberado previamente, puede causar un fallo.
       - Solución: Validar que `processor` no sea `NULL` antes de proceder.

   References:
     - Ninguna referencia externa específica.
 */
void JsonProcessor_destroy(JsonProcessor *processor) {
    if (processor != NULL) {
        free(processor);
        jsonProcessor_instance = NULL;
    }
}

/* Function: JsonProcessor_createJsonMessage
   Descripción:
     Crea un mensaje JSON a partir de una cadena de texto proporcionada. El mensaje se estructura con
     los campos "command" y "tipoCliente".

   Params:
     processor - Puntero al procesador JSON (`JsonProcessor *`) que se utiliza para generar el mensaje.
     message - Cadena de texto que contiene el valor del campo "tipoCliente".

   Returns:
     - char*: Una cadena de texto con el mensaje JSON generado.
       Retorna `NULL` si ocurre un error durante la creación o conversión del JSON.

   Restriction:
     - `processor` debe estar correctamente inicializado mediante `JsonProcessor_create`.
     - `message` debe ser una cadena válida y no nula.
     - La cadena devuelta debe ser liberada por el llamador utilizando `free` después de su uso.

   Example:
     char *jsonMessage = JsonProcessor_createJsonMessage(processor, "jugador");
     if (jsonMessage != NULL) {
         printf("Mensaje JSON generado: %s\n", jsonMessage);
         free(jsonMessage);
     }

   Problems:
     - Problema: Si `processor` es `NULL`, no se puede generar el mensaje JSON.
       - Solución: Registrar un error y retornar `NULL`.
     - Problema: Si `cJSON_CreateObject` falla, no se crea el objeto JSON.
       - Solución: Registrar un error crítico y manejarlo adecuadamente.
     - Problema: Si `cJSON_PrintUnformatted` falla, no se convierte el JSON a cadena.
       - Solución: Liberar el objeto JSON y retornar `NULL`.

   References:
     - cJSON Documentation: https://github.com/DaveGamble/cJSON
 */
char *JsonProcessor_createJsonMessage(JsonProcessor *processor, const char *message) {
    if (processor == NULL) {
        savelog_error("JsonProcessor no inicializado\n");
        return NULL;
    }

    // Crear un objeto JSON
    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        savelog_fatal("Error al crear el objeto JSON\n");
        return NULL;
    }

    // Añadir el mensaje al objeto JSON
    cJSON_AddStringToObject(json, "command", "tipoCliente");
    cJSON_AddStringToObject(json, "tipoCliente", message);

    // Convertir el objeto JSON a una cadena de texto
    char *jsonString = cJSON_PrintUnformatted(json);
    if (jsonString == NULL) {
        savelog_error("Error al convertir el objeto JSON a cadena\n");
        cJSON_Delete(json); // Liberar el objeto JSON en caso de error
        return NULL;
    }

    // Liberar el objeto JSON ya que no lo necesitamos más
    cJSON_Delete(json);

    return jsonString;
}


/* Function: JsonProcessor_createJsonPlayerName
   Descripción:
     Crea un mensaje JSON que incluye el nombre del jugador. El mensaje tiene los campos "command",
     "tipoCliente" y "playerName".

   Params:
     processor - Puntero al procesador JSON (`JsonProcessor *`) que se utiliza para generar el mensaje.
     playerName - Cadena de texto que contiene el nombre del jugador.

   Returns:
     - char*: Una cadena de texto con el mensaje JSON generado.
       Retorna `NULL` si ocurre un error durante la creación o conversión del JSON.

   Restriction:
     - `processor` debe estar correctamente inicializado mediante `JsonProcessor_create`.
     - `playerName` debe ser una cadena válida y no nula.
     - La cadena devuelta debe ser liberada por el llamador utilizando `free` después de su uso.

   Example:
     char *jsonMessage = JsonProcessor_createJsonPlayerName(processor, "Jugador1");
     if (jsonMessage != NULL) {
         printf("Mensaje JSON generado: %s\n", jsonMessage);
         free(jsonMessage);
     }

   Problems:
     - Problema: Si `processor` es `NULL`, no se puede generar el mensaje JSON.
       - Solución: Registrar un error y retornar `NULL`.
     - Problema: Si `cJSON_CreateObject` falla, no se crea el objeto JSON.
       - Solución: Registrar un error crítico y manejarlo adecuadamente.
     - Problema: Si `cJSON_PrintUnformatted` falla, no se convierte el JSON a cadena.
       - Solución: Liberar el objeto JSON y retornar `NULL`.

   References:
     - cJSON Documentation: https://github.com/DaveGamble/cJSON

*/

char *JsonProcessor_createJsonPlayerName(JsonProcessor *processor, const char *playerName) {
    if (processor == NULL) {
        savelog_error("JsonProcessor no inicializado\n");
        return NULL;
    }

    // Crear un objeto JSON
    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        savelog_fatal("Error al crear el objeto JSON\n");
        return NULL;
    }

    // Añadir los campos al objeto JSON
    cJSON_AddStringToObject(json, "command", "tipoCliente");
    cJSON_AddStringToObject(json, "tipoCliente", "player");
    cJSON_AddStringToObject(json, "playerName", playerName);

    // Convertir el objeto JSON a una cadena de texto
    char *jsonString = cJSON_PrintUnformatted(json);
    if (jsonString == NULL) {
        savelog_error("Error al convertir el objeto JSON a cadena\n");
        cJSON_Delete(json); // Liberar el objeto JSON en caso de error
        return NULL;
    }

    // Liberar el objeto JSON ya que no lo necesitamos más
    cJSON_Delete(json);

    return jsonString;
}

/* Function: JsonProcessor_createJsonChoosenPlayer
   Descripción:
     Crea un mensaje JSON que incluye el ID del jugador elegido por el espectador. El mensaje contiene
     los campos "command" y "jugadorId".

   Params:
     processor - Puntero al procesador JSON (`JsonProcessor *`) que se utiliza para generar el mensaje.
     playerId - Cadena de texto que contiene el ID del jugador elegido.

   Returns:
     - char*: Una cadena de texto con el mensaje JSON generado.
       Retorna `NULL` si ocurre un error durante la creación o conversión del JSON.

   Restriction:
     - `processor` debe estar correctamente inicializado mediante `JsonProcessor_create`.
     - `playerId` debe ser una cadena válida y no nula.
     - La cadena devuelta debe ser liberada por el llamador utilizando `free` después de su uso.

   Example:
     char *jsonMessage = JsonProcessor_createJsonChoosenPlayer(processor, "12345");
     if (jsonMessage != NULL) {
         printf("Mensaje JSON generado: %s\n", jsonMessage);
         free(jsonMessage);
     }

   Problems:
     - Problema: Si `processor` es `NULL`, no se puede generar el mensaje JSON.
       - Solución: Registrar un error y retornar `NULL`.
     - Problema: Si `cJSON_CreateObject` falla, no se crea el objeto JSON.
       - Solución: Registrar un error crítico y manejarlo adecuadamente.
     - Problema: Si `cJSON_PrintUnformatted` falla, no se convierte el JSON a cadena.
       - Solución: Liberar el objeto JSON y retornar `NULL`.

   References:
     - cJSON Documentation: https://github.com/DaveGamble/cJSON
*/

char *JsonProcessor_createJsonChoosenPlayer(JsonProcessor *processor, const char *playerId) {
    if (processor == NULL) {
        savelog_error("JsonProcessor no inicializado\n");
        return NULL;
    }

    // Crear un objeto JSON
    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        savelog_fatal("Error al crear el objeto JSON\n");
        return NULL;
    }

    // Añadir los campos al objeto JSON
    cJSON_AddStringToObject(json, "command", "GameSpectator");
    cJSON_AddStringToObject(json, "jugadorId", playerId);

    // Convertir el objeto JSON a una cadena de texto
    char *jsonString = cJSON_PrintUnformatted(json);
    if (jsonString == NULL) {
        savelog_error("Error al convertir el objeto JSON a cadena\n");
        cJSON_Delete(json); // Liberar el objeto JSON en caso de error
        return NULL;
    }

    // Liberar el objeto JSON ya que no lo necesitamos más
    cJSON_Delete(json);

    return jsonString;
}

/* Function: JsonProcessor_createJsonGetListPlayers
   Descripción:
     Crea un mensaje JSON que solicita la lista de jugadores disponibles. El mensaje contiene
     los campos "command", "tipoCliente" y "playerName".

   Params:
     processor - Puntero al procesador JSON (`JsonProcessor *`) que se utiliza para generar el mensaje.

   Returns:
     - char*: Una cadena de texto con el mensaje JSON generado.
       Retorna `NULL` si ocurre un error durante la creación o conversión del JSON.

   Restriction:
     - `processor` debe estar correctamente inicializado mediante `JsonProcessor_create`.
     - La cadena devuelta debe ser liberada por el llamador utilizando `free` después de su uso.

   Example:
     char *jsonMessage = JsonProcessor_createJsonGetListPlayers(processor);
     if (jsonMessage != NULL) {
         printf("Mensaje JSON generado: %s\n", jsonMessage);
         free(jsonMessage);
     }

   Problems:
     - Problema: Si `processor` es `NULL`, no se puede generar el mensaje JSON.
       - Solución: Registrar un error y retornar `NULL`.
     - Problema: Si `cJSON_CreateObject` falla, no se crea el objeto JSON.
       - Solución: Registrar un error crítico y manejarlo adecuadamente.
     - Problema: Si `cJSON_PrintUnformatted` falla, no se convierte el JSON a cadena.
       - Solución: Liberar el objeto JSON y retornar `NULL`.

   References:
     - cJSON Documentation: https://github.com/DaveGamble/cJSON
*/
char *JsonProcessor_createJsonGetListPlayers(JsonProcessor *processor) {
    if (processor == NULL) {
        savelog_error("JsonProcessor no inicializado\n");
        return NULL;
    }

    // Crear un objeto JSON
    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        savelog_fatal("Error al crear el objeto JSON\n");
        return NULL;
    }

    // Añadir los campos al objeto JSON
    cJSON_AddStringToObject(json, "command", "tipoCliente");
    cJSON_AddStringToObject(json, "tipoCliente", "spectador");
    cJSON_AddStringToObject(json, "playerName", "Observer");

    // Convertir el objeto JSON a una cadena de texto
    char *jsonString = cJSON_PrintUnformatted(json);
    if (jsonString == NULL) {
        savelog_error("Error al convertir el objeto JSON a cadena\n");
        cJSON_Delete(json); // Liberar el objeto JSON en caso de error
        return NULL;
    }

    // Liberar el objeto JSON ya que no lo necesitamos más
    cJSON_Delete(json);

    return jsonString;
}

/* Function: JsonProcessor_processJsonMessage
   Descripción:
     Procesa un mensaje JSON recibido, extrayendo el valor del campo "message". Devuelve el mensaje como
     una nueva cadena de texto, la cual debe ser liberada por el llamador.

   Params:
     processor - Puntero al procesador JSON (`JsonProcessor *`) que se utiliza para procesar el mensaje.
     jsonMessage - Cadena de texto que contiene el mensaje JSON recibido.

   Returns:
     - char*: Una cadena de texto con el valor del campo "message".
       Retorna `NULL` si ocurre un error durante el procesamiento.

   Restriction:
     - `processor` debe estar correctamente inicializado mediante `JsonProcessor_create`.
     - `jsonMessage` debe ser una cadena válida y no nula.
     - La cadena devuelta debe ser liberada por el llamador utilizando `free` después de su uso.

   Example:
     char *processedMessage = JsonProcessor_processJsonMessage(processor, "{\"message\":\"Hola\"}");
     if (processedMessage != NULL) {
         printf("Mensaje procesado: %s\n", processedMessage);
         free(processedMessage);
     }

   Problems:
     - Problema: Si `processor` es `NULL`, no se puede procesar el mensaje JSON.
       - Solución: Registrar un error y retornar `NULL`.
     - Problema: Si el mensaje JSON recibido no es válido, no se puede extraer el campo "message".
       - Solución: Registrar un error y manejarlo adecuadamente.
     - Problema: Si `strdup` falla, no se puede duplicar el valor del mensaje.
       - Solución: Manejar el error y registrar un mensaje de advertencia.

   References:
     - cJSON Documentation: https://github.com/DaveGamble/cJSON
 */
char *JsonProcessor_processJsonMessage(JsonProcessor *processor, const char *jsonMessage) {
    if (processor == NULL) {
        savelog_error("JsonProcessor no inicializado\n");
        return NULL;
    }

    // Parsear el mensaje JSON recibido
    cJSON *json = cJSON_Parse(jsonMessage);
    if (json == NULL) {
        savelog_error("Error al parsear el JSON recibido\n");
        return NULL;
    }

    // Extraer el campo "message" del JSON
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");
    if (!cJSON_IsString(message) || (message->valuestring == NULL)) {
        savelog_error("Error: El campo 'message' no es válido o está vacío\n");
        cJSON_Delete(json); // Liberar el objeto JSON
        return NULL;
    }

    // Duplicar el valor del mensaje para devolverlo (ya que se debe liberar la memoria original)
    char *processedMessage = strdup(message->valuestring);
    if (processedMessage == NULL) {
        savelog_error("Error al duplicar el mensaje procesado\n");
    }

    // Liberar el objeto JSON
    cJSON_Delete(json);

    return processedMessage;
}