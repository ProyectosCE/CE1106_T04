#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "configuracion.h"
#include "ini.h"

// Función handler para procesar los valores del archivo ini
static int handler(void* user, const char* section, const char* name, const char* value) {
    ConfiguracionData* data = (ConfiguracionData*)user;

    if (strcmp(section, "socket") == 0) {
        if (strcmp(name, "ip") == 0) {
            strncpy(data->ip, value, sizeof(data->ip) - 1);
            data->ip[sizeof(data->ip) - 1] = '\0';  // Asegurar null-terminado
        } else if (strcmp(name, "port") == 0) {
            data->puerto = atoi(value);
        }
    }
    return 1;
}

// Constructor simulado
Configuracion* crear_configuracion() {
    Configuracion* config = (Configuracion*)malloc(sizeof(Configuracion));
    if (config) {
        config->filename = "settings.ini";
    }
    return config;
}

// Método para leer el valor de la IP
const char* leer_ip(Configuracion* config) {
    ConfiguracionData data = {"", 0};  // Inicializamos en vacío

    if (ini_parse(config->filename, handler, &data) < 0) {
        printf("No se puede abrir el archivo ini: %s\n", config->filename);
        return NULL;
    }

    return strdup(data.ip);  // Devolvemos una copia de la IP
}

// Método para leer el valor del puerto
int leer_puerto(Configuracion* config) {
    ConfiguracionData data = {"", 0};  // Inicializamos en vacío

    if (ini_parse(config->filename, handler, &data) < 0) {
        printf("No se puede abrir el archivo ini: %s\n", config->filename);
        return -1;
    }

    return data.puerto;
}

// Destructor simulado
void destruir_configuracion(Configuracion* config) {
    if (config) {
        free(config);
    }
}
