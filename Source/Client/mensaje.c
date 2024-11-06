// ARCHIVO BLOQUE.H
//NO OCUPA BLOQUE.C

#ifndef BLOQUE_H
#define BLOQUE_H

// Estructura que representa un bloque en el juego
typedef struct {
    int fila;
    int columna;
    char *poder;
} Bloque;

#endif

// ARCHIVO MENSAJE.H
//NO OCUPA MENSAJE.C
#ifndef MENSAJE_H
#define MENSAJE_H

#include "bloque.h"

// Estructura que representa un mensaje JSON procesado
typedef struct {
    char *estado;
    char *comando;
    Bloque bloque;
} Mensaje;

// Función para liberar la memoria de un Mensaje
void Mensaje_destroy(Mensaje *mensaje);

#endif

//JSONPROCESSOR.H
// Se debe añadir estas lineas en jsonprocessor.h
#include "mensaje.h"

// Función para procesar el mensaje JSON
Mensaje *JsonProcessor_processJsonMessage(const char *jsonMessage);

// Función para procesar el mensaje JSON y devolver una estructura Mensaje
Mensaje *JsonProcessor_processJsonMessage(const char *jsonMessage) {
    // Parsear el mensaje JSON recibido
    cJSON *json = cJSON_Parse(jsonMessage);
    if (json == NULL) {
        fprintf(stderr, "Error al parsear el JSON recibido\n");
        return NULL;
    }

    // Crear una instancia de Mensaje
    Mensaje *mensaje = (Mensaje *)malloc(sizeof(Mensaje));
    if (mensaje == NULL) {
        fprintf(stderr, "Error al asignar memoria para el mensaje\n");
        cJSON_Delete(json);
        return NULL;
    }

    // Extraer el campo "estado"
    cJSON *estado = cJSON_GetObjectItemCaseSensitive(json, "estado");
    if (cJSON_IsString(estado) && (estado->valuestring != NULL)) {
        mensaje->estado = strdup(estado->valuestring);
    } else {
        mensaje->estado = NULL;
    }

    // Extraer el campo "comando"
    cJSON *comando = cJSON_GetObjectItemCaseSensitive(json, "comando");
    if (cJSON_IsString(comando) && (comando->valuestring != NULL)) {
        mensaje->comando = strdup(comando->valuestring);
    } else {
        mensaje->comando = NULL;
    }

    // Extraer el objeto "bloque"
    cJSON *bloque = cJSON_GetObjectItemCaseSensitive(json, "bloque");
    if (cJSON_IsObject(bloque)) {
        // Extraer "fila" dentro de "bloque"
        cJSON *fila = cJSON_GetObjectItemCaseSensitive(bloque, "fila");
        mensaje->bloque.fila = cJSON_IsNumber(fila) ? fila->valueint : -1;

        // Extraer "columna" dentro de "bloque"
        cJSON *columna = cJSON_GetObjectItemCaseSensitive(bloque, "columna");
        mensaje->bloque.columna = cJSON_IsNumber(columna) ? columna->valueint : -1;

        // Extraer "poder" dentro de "bloque"
        cJSON *poder = cJSON_GetObjectItemCaseSensitive(bloque, "poder");
        if (cJSON_IsString(poder) && (poder->valuestring != NULL)) {
            mensaje->bloque.poder = strdup(poder->valuestring);
        } else {
            mensaje->bloque.poder = NULL;
        }
    } else {
        mensaje->bloque.fila = -1;
        mensaje->bloque.columna = -1;
        mensaje->bloque.poder = NULL;
    }

    // Liberar el objeto JSON
    cJSON_Delete(json);

    return mensaje;
}

// Función para liberar la memoria de un Mensaje
void Mensaje_destroy(Mensaje *mensaje) {
    if (mensaje != NULL) {
        free(mensaje->estado);
        free(mensaje->comando);
        free(mensaje->bloque.poder);
        free(mensaje);
    }
}



//EJEMPLO DE UN JSON DE BLOQUES:

{
  "estado": "activo",
  "comando": "actualizar_bloque",
  "bloque": {
    "fila": 3,
    "columna": 5,
    "poder": "super_fuerza"
  }
}





