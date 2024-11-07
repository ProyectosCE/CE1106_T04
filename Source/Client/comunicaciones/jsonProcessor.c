#include "jsonProcessor.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "jsonProcessor.h"
#include <cjson/cJSON.h>
#include <stdlib.h>

/*
 * Crear un mensaje JSON para el tipo de cliente usando cJSON
 */
char *JsonProcessor_createClientTypeMessage(const char *clientType) {
    if (clientType == NULL) {
        return NULL;
    }

    // Crear el objeto JSON
    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        return NULL;
    }

    // Añadir los campos "command" y "tipoCliente" al objeto JSON
    cJSON_AddStringToObject(json, "command", "tipoCliente");
    cJSON_AddStringToObject(json, "tipoCliente", clientType);

    // Convertir el objeto JSON a una cadena de texto
    char *message = cJSON_PrintUnformatted(json);
    if (message == NULL) {
        cJSON_Delete(json);
        return NULL;
    }

    // Liberar el objeto JSON ya que no se necesita más
    //cJSON_Delete(json);

    return message;
}


char *JsonProcessor_processJsonMessage(const char *jsonMessage) {
    cJSON *json = cJSON_Parse(jsonMessage);
    if (json == NULL) {
        return NULL;
    }

    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");
    char *processedMessage = NULL;

    if (cJSON_IsString(message) && (message->valuestring != NULL)) {
        processedMessage = strdup(message->valuestring);
    }

    cJSON_Delete(json);
    return processedMessage;
}