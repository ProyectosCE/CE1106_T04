#include "jsonProcessor.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "cjson/cJSON.h"

// Función para convertir un mensaje de cadena a JSON
char *processToJson(const char *message) {
    if (message == NULL || strlen(message) == 0) {
        printf("Error: El mensaje está vacío o es nulo\n");
        return NULL;
    }

    // Crear un objeto JSON
    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        printf("Error al crear el objeto JSON\n");
        return NULL;
    }

    // Agregar el mensaje como parte del JSON
    cJSON_AddStringToObject(json, "message", message);

    // Convertir el objeto JSON a una cadena
    char *jsonString = cJSON_Print(json);
    if (jsonString == NULL) {
        printf("Error al crear el mensaje JSON\n");
        cJSON_Delete(json);
        return NULL;
    }

    // Limpiar el objeto JSON ya que solo necesitamos la cadena resultante
    cJSON_Delete(json);

    return jsonString;
}

// Función para procesar un mensaje JSON entrante
char *processFromJson(const char *jsonMessage) {
    if (jsonMessage == NULL || strlen(jsonMessage) == 0) {
        printf("Error: El mensaje JSON recibido está vacío o es nulo\n");
        return NULL;
    }

    // Parsear el mensaje JSON
    cJSON *json = cJSON_Parse(jsonMessage);
    if (json == NULL) {
        printf("Error al parsear el JSON recibido\n");
        return NULL;
    }

    // Obtener el valor de "message" del JSON
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");
    if (!cJSON_IsString(message) || (message->valuestring == NULL)) {
        printf("Error: No se encontró un campo 'message' válido en el JSON\n");
        cJSON_Delete(json);
        return NULL;
    }

    // Crear una copia de la cadena de mensaje recibida
    char *processedMessage = strdup(message->valuestring);

    // Limpiar el objeto JSON
    cJSON_Delete(json);

    return processedMessage;
}
