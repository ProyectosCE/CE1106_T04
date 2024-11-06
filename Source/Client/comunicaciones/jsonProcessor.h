#ifndef JSON_PROCESSOR_H
#define JSON_PROCESSOR_H

#include "cjson/cJSON.h"

typedef struct {
    // Puede agregar más atributos si es necesario en el futuro
} JsonProcessor;

// Constructor y Destructor
JsonProcessor *JsonProcessor_create();
void JsonProcessor_destroy(JsonProcessor *processor);

// Métodos de la clase
char *JsonProcessor_createJsonMessage(JsonProcessor *processor, const char *message);
char *JsonProcessor_processJsonMessage(JsonProcessor *processor, const char *jsonMessage);

char *JsonProcessor_createJsonPlayerName(JsonProcessor *processor, const char *playerName);

#endif // JSON_PROCESSOR_H
