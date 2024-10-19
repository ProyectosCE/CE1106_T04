#include "jsonProcessor.h"
#include "cjson/cJSON.h"
#include <stdio.h>
#include <string.h>

/*
 * Convierte un mensaje de formato C a JSON
 */
char *processToJson(const char *message) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "message", message);
    char *jsonString = cJSON_Print(json);
    cJSON_Delete(json);
    return jsonString;
}

/*
 * Convierte un mensaje JSON a formato C
 */
char *processFromJson(const char *jsonMessage) {
    cJSON *json = cJSON_Parse(jsonMessage);
    if (json == NULL) {
        printf("Error al parsear el JSON\n");
        return NULL;
    }

    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");
    char *processedMessage = strdup(message->valuestring);

    cJSON_Delete(json); // Liberar memoria JSON
    return processedMessage;
}
