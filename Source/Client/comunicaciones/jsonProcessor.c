#include "jsonProcessor.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Constructor: Inicializa el procesador JSON
 */
JsonProcessor *JsonProcessor_create() {
    JsonProcessor *processor = (JsonProcessor *)malloc(sizeof(JsonProcessor));
    if (processor == NULL) {
        printf("Error al asignar memoria para JsonProcessor\n");
        return NULL;
    }

    // Inicializar cualquier otro campo si es necesario

    return processor;
}

/*
 * Destructor: Libera los recursos de JsonProcessor
 */
void JsonProcessor_destroy(JsonProcessor *processor) {
    if (processor != NULL) {
        // Liberar cualquier recurso adicional si es necesario
        free(processor);
    }
}

/*
 * Método para crear un mensaje JSON a partir de una cadena de texto
 */
char *JsonProcessor_createJsonMessage(JsonProcessor *processor, const char *message) {
    // Crear un objeto JSON
    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        printf("Error al crear el objeto JSON\n");
        return NULL;
    }

    // Añadir el mensaje al objeto JSON
    cJSON_AddStringToObject(json, "message", message);

    // Convertir el objeto JSON a una cadena de texto
    char *jsonString = cJSON_PrintUnformatted(json);
    if (jsonString == NULL) {
        printf("Error al convertir el objeto JSON a cadena\n");
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
    // Parsear el mensaje JSON recibido
    cJSON *json = cJSON_Parse(jsonMessage);
    if (json == NULL) {
        printf("Error al parsear el JSON recibido\n");
        return NULL;
    }

    // Extraer el campo "message" del JSON
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");
    if (!cJSON_IsString(message) || (message->valuestring == NULL)) {
        printf("Error: El campo 'message' no es válido o está vacío\n");
        cJSON_Delete(json); // Liberar el objeto JSON
        return NULL;
    }

    // Duplicar el valor del mensaje para devolverlo (ya que se debe liberar la memoria original)
    char *processedMessage = strdup(message->valuestring);
    if (processedMessage == NULL) {
        printf("Error al duplicar el mensaje procesado\n");
    }

    // Liberar el objeto JSON
    cJSON_Delete(json);

    return processedMessage;
}
