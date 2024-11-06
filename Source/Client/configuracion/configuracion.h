#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#define MAX_HASH_SIZE 1024

typedef enum {
    CONFIG_TYPE_INT,
    CONFIG_TYPE_FLOAT,
    CONFIG_TYPE_STRING
} ConfigType;


typedef struct ConfigEntry {
    char* key;
    union {
        int int_val;
        float float_val;
        char* str_val;
    } value;
    ConfigType type;
    struct ConfigEntry* next;
} ConfigEntry;

typedef struct {
    ConfigEntry* entries[MAX_HASH_SIZE];
    int cargado;
} Configuracion;

// Funciones de configuración
void inicializar_configuracion();
int get_config_int(const char* key);
float get_config_float(const char* key);
const char* get_config_string(const char* key);
void destruir_configuracion();

#endif