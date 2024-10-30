#ifndef CONFIGURACION_H
#define CONFIGURACION_H

typedef struct {
    const char* filename;
} Configuracion;

typedef struct {
    char ip[16];
    int puerto;
} ConfiguracionData;

// Crea una nueva configuración con el nombre del archivo .ini
Configuracion* crear_configuracion_file(const char* filename);

Configuracion* crear_configuracion();

// Lee el valor de la IP del archivo .ini
const char* leer_ip(Configuracion* config);

// Lee el valor del puerto del archivo .ini
int leer_puerto(Configuracion* config);

// Libera la memoria asociada a la configuración
void destruir_configuracion(Configuracion* config);

#endif // CONFIGURACION_H
