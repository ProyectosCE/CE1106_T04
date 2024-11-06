#include "jsonProcessor.h"
#include "../logs/saveLog.h"
#include <stdlib.h>
#include <string.h>

// Puntero estático para almacenar la única instancia de JsonProcessor
static JsonProcessor *jsonProcessor_instance = NULL;

/*
 * Constructor: Inicializa el procesador JSON
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

/*
 * Destructor: Libera los recursos de JsonProcessor
 */
void JsonProcessor_destroy(JsonProcessor *processor) {
    if (processor != NULL) {
        free(processor);
        jsonProcessor_instance = NULL;
    }
}

/*
 * Método para crear un mensaje JSON a partir de una cadena de texto
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

/*
 * Método para procesar un mensaje JSON recibido
 * Este método toma una cadena JSON y devuelve un mensaje procesado como cadena de texto.
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