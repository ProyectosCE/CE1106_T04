#ifndef JSON_PROCESSOR_H
#define JSON_PROCESSOR_H

// Crear un mensaje JSON para especificar el tipo de cliente
char *JsonProcessor_createClientTypeMessage(const char *clientType);
char *JsonProcessor_processJsonMessage(const char *jsonMessage);

#endif // JSON_PROCESSOR_H
