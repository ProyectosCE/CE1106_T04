/*
================================== LICENCIA ==================================================
MIT License
Copyright (c) 2024 José Bernardo Barquero Bonilla,
                   Jose Eduardo Campos Salazar,
                   Jimmy Feng Feng,
                   Alexander Montero Vargas
Consulta el archivo LICENSE para más detalles.
==============================================================================================
*/

// BIBLIOTECAS DE PROYECTO

#include "configuracion.h"
#include "../logs/saveLog.h"

// BIBLIOTECAS EXTERNAS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ini.h"


static Configuracion config = { .cargado = 0 };

// Función hash simple
/* Function: hash
   Descripción:
     Calcula un hash simple para una cadena de texto utilizando desplazamientos y suma acumulativa.

   Params:
     key - Cadena de texto que será procesada para calcular el hash.

   Returns:
     - unsigned int: Valor hash de la clave, limitado al tamaño máximo de la tabla hash (`MAX_HASH_SIZE`).

   Restriction:
     - `key` debe ser una cadena válida y no nula.

   Example:
     unsigned int index = hash("example.key");
     // Calcula el índice hash para la clave.

   Problems:
     - Problema: Si `key` es nulo, el comportamiento es indefinido.
       - Solución: Validar la entrada antes de llamar a la función.

   References:
     - Ninguna referencia externa específica.
*/
static unsigned int hash(const char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + *key++;
    }
    return hash % MAX_HASH_SIZE;
}


// Función para agregar una entrada en la tabla hash
/* Function: add_entry
   Descripción:
     Agrega una entrada a la tabla hash global de configuración, determinando el tipo de dato (cadena, entero o flotante)
     del valor y manejando posibles colisiones.

   Params:
     key - Clave única de configuración, combinada de sección y nombre.
     value - Valor asociado a la clave.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `key` y `value` deben ser cadenas válidas y no nulas.
     - Se espera que la tabla hash (`config.entries`) esté inicializada antes de llamar a esta función.

   Example:
     add_entry("game.maxBalls", "5");
     // Agrega una entrada para el valor máximo de bolas en el juego.

   Problems:
     - Problema: Si no se puede asignar memoria para la entrada, el programa podría fallar.
       - Solución: Manejar errores de asignación de memoria.
     - Problema: Si `value` tiene un formato inesperado, puede no ser procesado correctamente.
       - Solución: Validar el formato de `value` antes de procesarlo.

   References:
     - Ninguna referencia externa específica.
*/
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
/* Function: handler
Descripción:
Procesa cada línea del archivo INI y agrega las claves y valores correspondientes a la tabla hash global.

Params:
user - Puntero a datos de usuario (no utilizado en esta implementación).
section - Sección actual en el archivo INI.
name - Nombre de la clave dentro de la sección.
value - Valor asociado a la clave.

Returns:
- int: Retorna 1 para indicar éxito.

Restriction:
- `section`, `name`, y `value` deben ser cadenas válidas y no nulas.

Example:
handler(NULL, "game", "maxBalls", "5");
// Agrega una entrada "game.maxBalls" con valor "5".

Problems:
- Problema: Si la clave generada excede el tamaño del buffer, podría truncarse.
- Solución: Asegurarse de que el tamaño del buffer sea suficiente o manejar dinámicamente las claves largas.

References:
- Ninguna referencia externa específica.
*/
static int handler(void* user, const char* section, const char* name, const char* value) {
    char key[50];
    snprintf(key, sizeof(key), "%s.%s", section, name);
    add_entry(key, value);
    return 1;
}

/* Function: inicializar_configuracion
   Descripción:
     Carga la configuración desde un archivo INI usando un parser y almacena los valores en una tabla hash.

   Params:
     (Ninguno)

   Returns:
     - void: No retorna valores.

   Restriction:
     - El archivo INI debe existir y ser legible en la ruta especificada (`settings.ini`).
     - `handler` debe estar correctamente implementado para procesar las entradas del archivo.

   Example:
     inicializar_configuracion();
     // Carga la configuración del archivo settings.ini.

   Problems:
     - Problema: Si el archivo no se encuentra o es ilegible, no se cargará la configuración.
       - Solución: Registrar un error con `savelog_error`.

   References:
     - Ninguna referencia externa específica.
*/
void inicializar_configuracion() {
    if (config.cargado) return;

    const char* filename = "settings.ini";
    if (ini_parse(filename, handler, NULL) < 0) {
        savelog_error("No se puede abrir el archivo ini: %s", filename);
        return;
    }
    config.cargado = 1;
}

/* Function: get_config_string
   Descripción:
     Obtiene el valor asociado a una clave en formato de cadena desde la tabla hash de configuración.

   Params:
     key - Clave de configuración (ejemplo: "game.maxBalls").

   Returns:
     - const char*: Cadena de texto asociada a la clave, o NULL si no se encuentra.

   Restriction:
     - `key` debe ser una cadena válida y no nula.

   Example:
     const char* value = get_config_string("game.title");
     // Retorna el valor de la clave "game.title" si existe.

   Problems:
     - Problema: Si la configuración no está cargada, no se encontrarán las claves.
       - Solución: Llamar a `inicializar_configuracion` si no está cargada.

   References:
     - Ninguna referencia externa específica.
*/
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

/* Function: get_config_float
   Descripción:
     Obtiene el valor asociado a una clave en formato flotante desde la tabla hash de configuración.

   Params:
     key - Clave de configuración (ejemplo: "game.ballSpeedMultiplier").

   Returns:
     - float: Valor flotante asociado a la clave, o 0.0f si no se encuentra.

   Restriction:
     - `key` debe ser una cadena válida y no nula.

   Example:
     float value = get_config_float("game.ballSpeedMultiplier");
     // Retorna el valor de la clave "game.ballSpeedMultiplier" si existe.

   Problems:
     - Problema: Si la configuración no está cargada, no se encontrarán las claves.
       - Solución: Llamar a `inicializar_configuracion` si no está cargada.

   References:
     - Ninguna referencia externa específica.
*/
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

/* Function: get_config_int
   Descripción:
     Obtiene el valor asociado a una clave en formato entero desde la tabla hash de configuración.

   Params:
     key - Clave de configuración (ejemplo: "game.maxBalls").

   Returns:
     - int: Valor entero asociado a la clave, o 0 si no se encuentra.

   Restriction:
     - `key` debe ser una cadena válida y no nula.

   Example:
     int value = get_config_int("game.maxBalls");
     // Retorna el valor de la clave "game.maxBalls" si existe.

   Problems:
     - Problema: Si la configuración no está cargada, no se encontrarán las claves.
       - Solución: Llamar a `inicializar_configuracion` si no está cargada.

   References:
     - Ninguna referencia externa específica.
*/
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

/* Function: destruir_configuracion
Descripción:
Libera la memoria asignada dinámicamente para la configuración al finalizar el programa.

Params:
(Ninguno)

Returns:
- void: No retorna valores.

Restriction:
- Se debe llamar antes de finalizar el programa para evitar fugas de memoria.

Example:
destruir_configuracion();
// Limpia la memoria asignada para la configuración.

Problems:
- Problema: Si no se llama, se pueden producir fugas de memoria.
- Solución: Integrar esta función en la limpieza final del programa.

References:
- Ninguna referencia externa específica.
*/
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
