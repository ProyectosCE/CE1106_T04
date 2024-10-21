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
   Header: saveLog
   Este herader representa un evento de logging que se usa para encapsular la información relevante sobre un
   mensaje de log en el sistema SaveLog.

   Typedefs:
     - savelog_LogFn: Función de callback que maneja un evento de logging.
     - savelog_LockFn: Función de bloqueo que asegura la operación segura del logging en un entorno multihilo.

   Enumerations:
     - SAVELOG_TRACE: Nivel de log para trazas detalladas.
     - SAVELOG_DEBUG: Nivel de log para depuración.
     - SAVELOG_INFO: Nivel de log para información general.
     - SAVELOG_WARN: Nivel de log para advertencias.
     - SAVELOG_ERROR: Nivel de log para errores graves.
     - SAVELOG_FATAL: Nivel de log para errores fatales o críticos.

   Macros:
     - savelog_trace: Macro que genera un log de nivel TRACE.
     - savelog_debug: Macro que genera un log de nivel DEBUG.
     - savelog_info: Macro que genera un log de nivel INFO.
     - savelog_warn: Macro que genera un log de nivel WARN.
     - savelog_error: Macro que genera un log de nivel ERROR.
     - savelog_fatal: Macro que genera un log de nivel FATAL.

   Functions:
     - savelog_log: Función principal que registra un mensaje de log con el nivel de severidad, archivo y línea correspondientes.

   References:
     - Biblioteca original Log.c del usuario de github @rxi, puede accederlo en la url https://github.com/rxi/log.c/
*/
#ifndef SAVELOG_H
#define SAVELOG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

#define SAVELOG_VERSION "1.1.0" by ProyectosCE 2024

/*
   Struct: savelog_Event
   Representa un evento de logging.

   Members:
     - ap: va_list - Lista de argumentos de la función de formato variable que se utiliza en el log.
     - fmt: const char* - Formato de la cadena de texto que será registrada.
     - file: const char* - Nombre del archivo desde el que se ha llamado al sistema de logging.
     - time: struct tm* - Marca de tiempo del evento de logging.
     - udata: void* - Puntero de datos de usuario que se puede pasar a las funciones de callback.
     - line: int - Línea en el archivo de código fuente desde la que se generó el log.
     - level: int - Nivel de severidad del evento de logging (TRACE, DEBUG, INFO, WARN, ERROR, FATAL).

*/
typedef struct {
    va_list ap;             // Lista de argumentos de formato variable
    const char *fmt;        // Formato del mensaje
    const char *file;       // Nombre del archivo desde donde se registra el evento
    struct tm *time;        // Marca de tiempo del evento
    void *udata;            // Datos de usuario
    int line;               // Número de línea en el archivo
    int level;              // Nivel de severidad del log
} savelog_Event;

/*
   savelog_LogFn, savelog_LockFn
   Definen las funciones de callback y de bloqueo.
*/
typedef void (*savelog_LogFn)(savelog_Event *ev);
typedef void (*savelog_LockFn)(bool lock, void *udata);


enum { SAVELOG_TRACE, SAVELOG_DEBUG, SAVELOG_INFO, SAVELOG_WARN, SAVELOG_ERROR, SAVELOG_FATAL };

/*
   Macros Generan logs con diferentes niveles de severidad.
*/
#define savelog_trace(...) savelog_log(SAVELOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define savelog_debug(...) savelog_log(SAVELOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define savelog_info(...)  savelog_log(SAVELOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define savelog_warn(...)  savelog_log(SAVELOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define savelog_error(...) savelog_log(SAVELOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define savelog_fatal(...) savelog_log(SAVELOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

/*
   Function: savelog_log
   Función principal de logging. Registra el mensaje con su nivel de severidad.
*/
void savelog_log(int level, const char *file, int line, const char *fmt, ...);

#endif