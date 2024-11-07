#include "jsonProcessor.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Crear un mensaje JSON para el tipo de cliente
 */
char *JsonProcessor_createClientTypeMessage(const char *clientType) {
    if (clientType == NULL) {
        return NULL;
    }

    // Crear el mensaje en formato JSON
    const char *template = "{\"command\": \"tipoCliente\", \"tipoCliente\": \"%s\"}";
    int bufferSize = snprintf(NULL, 0, template, clientType) + 1;
    char *message = (char *)malloc(bufferSize);

    if (message != NULL) {
        snprintf(message, bufferSize, template, clientType);
    }

    return message;
}
