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

/*
   Implementation: SaveLog
   La estructura SaveLog está diseñada para proporcionar un sistema de logging configurable que permite almacenar
   mensajes de log en un archivo en el sistema de archivos. Facilita la configuración de diferentes niveles de log y
   callbacks personalizados para manejar eventos de logging.

   Struct:
   - <Callback>
   - <L>

   Functions:
     - <create_log_directory>: Crea la carpeta 'logs' en el directorio actual si no existe.
     - <open_log_file>: Abre el archivo `logs/project.log` donde se almacenarán los logs.
     - <file_callback>: Maneja el evento de logging escribiendo el mensaje en el archivo de log.
     - <lock>: Bloquea el sistema de logging para evitar condiciones de carrera en sistemas multihilo.
     - <unlock>: Desbloquea el sistema de logging.
     - <init_event>: Inicializa un evento de logging con la hora actual y otros datos relevantes.
     - <savelog_log>: La función principal de logging. Crea un evento de logging, maneja el archivo de log y ejecuta los callbacks.

   Example:

       --- Code
    // Generar un log de ejemplo
     savelog_log(SAVELOG_INFO, __FILE__, __LINE__, "Este es un mensaje de log de prueba.");
    ---

   Problems:
     Durante la implementación, uno de los problemas fue asegurarse de que la carpeta `logs`
     siempre exista antes de intentar abrir el archivo de log. Esto se solucionó con la función
     `create_log_directory()` que verifica si la carpeta existe y la crea si es necesario.

   References:
     - Biblioteca original Log.c del usuario de github @rxi, puede accederlo en la url https://github.com/rxi/log.c/
*/


#include "saveLog.h"
#include <sys/stat.h>  // Para crear la carpeta

#define MAX_CALLBACKS 32
#define LOG_DIR "logs"
#define LOG_FILE "logs/project.log"

/*
   Struct: Callback
   Estructura que representa una función de callback de logging que se ejecuta cuando ocurre un evento de logging.

   Members:
     fn: savelog_LogFn - Puntero a la función de callback que maneja un evento de logging.
     udata: void* - Datos de usuario que se pasan al callback. Este valor puede ser un puntero a cualquier tipo de estructura o información que el usuario necesite en el callback.
     level: int - Nivel mínimo de severidad para que se ejecute este callback. Solo los eventos de logging con un nivel mayor o igual a este valor activarán el callback.
*/
typedef struct {
    savelog_LogFn fn;
    void *udata;
    int level;
} Callback;

/*
   Struct: L
   Estructura estática global que mantiene el estado del sistema de logging. Esta estructura centraliza toda la
   configuración y estado actual del sistema de logging.

   Members:
     udata: void* - Datos de usuario que se pasan a la función de bloqueo. Puede contener cualquier información adicional requerida para las operaciones de sincronización.
     lock: savelog_LockFn - Función de bloqueo para asegurar la sincronización en sistemas multihilo. Esta función bloquea y desbloquea el acceso al sistema de logging para evitar condiciones de carrera.
     level: int - Nivel mínimo de logging. Solo los mensajes con un nivel mayor o igual a este valor serán registrados.
     quiet: bool - Indica si la salida de logs a la consola está deshabilitada. Si es `true`, no se imprimirán logs en la consola.
     callbacks[MAX_CALLBACKS]: Callback[] - Arreglo de estructuras `Callback` que contiene los callbacks registrados. Cada callback maneja eventos de logging y puede ser configurado con un nivel mínimo de severidad.
     log_file: FILE* - Puntero al archivo donde se escriben los logs. Se abre cuando el sistema de logging está configurado para escribir en un archivo.
*/
static struct {
    void *udata;                   // Datos de usuario para la función de bloqueo
    savelog_LockFn lock;           // Función de bloqueo para sincronización en sistemas multihilo
    int level;                     // Nivel mínimo de logging para registrar mensajes
    bool quiet;                    // Si es true, se deshabilita la salida a consola
    Callback callbacks[MAX_CALLBACKS];  // Arreglo de callbacks para manejar eventos de logging
    FILE *log_file;                // Archivo de log donde se guardan los mensajes
} L;



/*
   Array: level_strings
   Arreglo de cadenas de texto que representa los diferentes niveles de severidad de log disponibles en el sistema SaveLog. Cada nivel de log está asociado a un índice en el arreglo, desde TRACE hasta FATAL.

   Members:
     - "TRACE": Representa el nivel más bajo de logging, utilizado para trazas detalladas.
     - "DEBUG": Utilizado para mensajes de depuración.
     - "INFO": Mensajes de información general.
     - "WARN": Mensajes de advertencia.
     - "ERROR": Mensajes de error que no detienen la ejecución del programa.
     - "FATAL": Mensajes de error crítico o fatal que pueden detener la ejecución del programa.
*/

static const char *level_strings[] = {
        "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

/*
   Array: level_colors
   (Solo se utiliza si está definido LOG_USE_COLOR)
   Arreglo de cadenas que contiene códigos ANSI de color para resaltar los mensajes de log en la consola dependiendo de su nivel de severidad. Estos colores se aplican a la salida en la consola si el logging se configura con colores habilitados.

   Members:
     - "\x1b[94m": Color para TRACE.
     - "\x1b[36m": Color para DEBUG.
     - "\x1b[32m": Color para INFO.
     - "\x1b[33m": Color para WARN.
     - "\x1b[31m": Color para ERROR.
     - "\x1b[35m": Color para FATAL.
*/

#ifdef LOG_USE_COLOR
static const char *level_colors[] = {
  "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};
#endif

/*
   Function: create_log_directory
   Crea la carpeta 'logs' en el directorio actual del proyecto si esta no existe.
   Se utiliza la función `stat` para verificar si la carpeta ya existe, y en caso contrario, se crea con permisos 0700 (solo el propietario puede leer, escribir y ejecutar).

   Returns:
     - void
*/

static void create_log_directory() {
    struct stat st = {0};
    if (stat(LOG_DIR, &st) == -1) {
        mkdir(LOG_DIR, 0700);
    }
}

/*
   Function: open_log_file
   Abre el archivo de log en modo de adición (append) dentro de la carpeta `logs`.
   Si el archivo no puede ser abierto, se imprime un mensaje de error en la salida estándar de error.

   Returns:
     - void
*/

static void open_log_file() {
    create_log_directory();
    L.log_file = fopen(LOG_FILE, "a");
    if (!L.log_file) {
        fprintf(stderr, "Error al abrir el archivo de log %s\n", LOG_FILE);
        return;
    }
}

/*
   Function: file_callback
   Función de callback que maneja un evento de logging y lo escribe en un archivo de log.
   Verifica si el archivo de log está abierto; si no, lo abre. Luego, escribe el mensaje de log en el archivo, formateado con la fecha, el nivel de log, el archivo y la línea donde se generó el log.

   Parameters:
     - ev (savelog_Event*): El evento de logging que contiene el mensaje, el archivo de origen, el número de línea y otros detalles.

   Returns:
     - void
*/

static void file_callback(savelog_Event *ev) {
    if (!L.log_file) {
        open_log_file();
    }

    char buf[64];
    buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ev->time)] = '\0';
    fprintf(
            L.log_file, "%s %-5s %s:%d: ",
            buf, level_strings[ev->level], ev->file, ev->line);
    vfprintf(L.log_file, ev->fmt, ev->ap);
    fprintf(L.log_file, "\n");
    fflush(L.log_file);
}

/*
   Function: lock
   Llama a la función de bloqueo configurada para evitar condiciones de carrera en entornos multihilo.
   Esta función bloquea el sistema de logging antes de escribir los mensajes.

   Returns:
     - void
*/

static void lock(void) {
    if (L.lock) { L.lock(true, L.udata); }
}

/*
   Function: unlock
   Llama a la función de desbloqueo configurada para reanudar la ejecución en un entorno multihilo.
   Se utiliza después de escribir los mensajes de log.

   Returns:
     - void
*/

static void unlock(void) {
    if (L.lock) { L.lock(false, L.udata); }
}


/*
   Function: init_event
   Inicializa un evento y almacena la udata

    Parameters:
        ev - Evento de saveLog
        udata - data del usuario

   Returns:
     - void
*/
static void init_event(savelog_Event *ev, void *udata) {
    if (!ev->time) {
        time_t t = time(NULL);
        ev->time = localtime(&t);
    }
    ev->udata = udata;
}

/*
   Function: savelog_log
   Función principal de logging que registra un mensaje con su nivel de severidad, archivo y línea correspondientes.
   Ejecuta los callbacks que corresponden al nivel de logging y escribe en el archivo de log si está habilitado.

     Parameters:
        level - El nivel de logging.
        file - El archivo fuente desde donde se genera el log.
        line  - El número de línea en el archivo fuente.
        fmt - El formato del mensaje a registrar.
        ... - Lista de argumentos para el formato del mensaje.

   Returns:
     - void
*/

void savelog_log(int level, const char *file, int line, const char *fmt, ...) {
    savelog_Event ev = {
            .fmt   = fmt,
            .file  = file,
            .line  = line,
            .level = level,
    };

    lock();

    if (!L.quiet && level >= L.level) {
        init_event(&ev, stderr);
        va_start(ev.ap, fmt);
        file_callback(&ev);
        va_end(ev.ap);
    }

    for (int i = 0; i < MAX_CALLBACKS && L.callbacks[i].fn; i++) {
        Callback *cb = &L.callbacks[i];
        if (level >= cb->level) {
            init_event(&ev, cb->udata);
            va_start(ev.ap, fmt);
            cb->fn(&ev);
            va_end(ev.ap);
        }
    }

    unlock();
}
