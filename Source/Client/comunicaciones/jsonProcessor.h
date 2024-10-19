#ifndef JSON_PROCESSOR_H
#define JSON_PROCESSOR_H

/*
 * Funciones para convertir mensajes entre JSON y formato C
 */
char *processToJson(const char *message); // Convierte un mensaje de C a JSON
char *processFromJson(const char *jsonMessage); // Convierte un mensaje JSON a formato C

#endif // JSON_PROCESSOR_H
