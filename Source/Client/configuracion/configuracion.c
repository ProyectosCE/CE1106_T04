#include "configuracion.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ini.h"
#include "../logs/saveLog.h"

static Configuracion config = { .cargado = 0 };

// Función hash simple
static unsigned int hash(const char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + *key++;
    }
    return hash % MAX_HASH_SIZE;
}


// Función para agregar una entrada en la tabla hash
static void add_entry(const char* key, const char* value) {
    unsigned int index = hash(key);
    ConfigEntry* entry = (ConfigEntry*)malloc(sizeof(ConfigEntry));
    entry->key = strdup(key); // Duplica la clave

    // Determina el tipo de dato y almacena el valor
    if (value[0] == '"') {
        // Remove quotes from string value
        entry->value.str_val = strdup(value + 1);
        entry->value.str_val[strlen(entry->value.str_val) - 1] = '\0';
        entry->type = CONFIG_TYPE_STRING;
    } else if (sscanf(value, "%d", &entry->value.int_val) == 1) {
        entry->type = CONFIG_TYPE_INT;
    } else if (sscanf(value, "f%f", &entry->value.float_val) == 1) {
        entry->type = CONFIG_TYPE_FLOAT;
    } else {
        savelog_error("Unknown value type for key '%s'", key);
    }

    // Maneja colisiones en la tabla hash
    entry->next = config.entries[index];
    config.entries[index] = entry;
}

// Handler para procesar cada línea del archivo ini
static int handler(void* user, const char* section, const char* name, const char* value) {
    char key[50];
    snprintf(key, sizeof(key), "%s.%s", section, name);
    add_entry(key, value);
    return 1;
}

// Inicializa la configuración cargando el archivo ini
void inicializar_configuracion() {
    if (config.cargado) return;

    const char* filename = "settings.ini";
    if (ini_parse(filename, handler, NULL) < 0) {
        savelog_error("No se puede abrir el archivo ini: %s", filename);
        return;
    }
    config.cargado = 1;
}

// Busca el valor asociado a una clave como string
const char* get_config_string(const char* key) {
    if (!config.cargado) {
        inicializar_configuracion();
    }

    // Asegúrate de que el key esté en el formato correcto
    unsigned int index = hash(key);
    ConfigEntry* entry = config.entries[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0 && entry->type == CONFIG_TYPE_STRING) {
            return entry->value.str_val;
        }
        entry = entry->next;
    }
    return NULL;  // Devuelve NULL si la clave no existe
}

float get_config_float(const char* key) {
    if (!config.cargado) {
        inicializar_configuracion();
    }

    unsigned int index = hash(key);
    ConfigEntry* entry = config.entries[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0 && entry->type == CONFIG_TYPE_FLOAT) {
            return entry->value.float_val;
        }
        entry = entry->next;
    }
    return 0.0f;  // Devuelve 0.0f si la clave no existe o no es un float
}

int get_config_int(const char* key) {
    if (!config.cargado) {
        inicializar_configuracion();
    }
    unsigned int index = hash(key);
    ConfigEntry* entry = config.entries[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0 && entry->type == CONFIG_TYPE_INT) {
            return entry->value.int_val;
        }
        entry = entry->next;
    }
    return 0;  // Devuelve 0 si la clave no existe o no es un entero

}

// Limpia la memoria al final del programa
void destruir_configuracion() {
    for (int i = 0; i < MAX_HASH_SIZE; i++) {
        ConfigEntry* entry = config.entries[i];
        while (entry) {
            ConfigEntry* temp = entry;
            entry = entry-> next;
            free(temp->key);
            if (temp->type == CONFIG_TYPE_STRING) {
                free(temp->value.str_val);
            }
            free(temp);
        }
    }
}
