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
#ifndef SAVELOG_H
#define SAVELOG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

#define SAVELOG_VERSION "0.1.0"

typedef struct {
    va_list ap;
    const char *fmt;
    const char *file;
    struct tm *time;
    void *udata;
    int line;
    int level;
} savelog_Event;

typedef void (*savelog_LogFn)(savelog_Event *ev);
typedef void (*savelog_LockFn)(bool lock, void *udata);

enum { SAVELOG_TRACE, SAVELOG_DEBUG, SAVELOG_INFO, SAVELOG_WARN, SAVELOG_ERROR, SAVELOG_FATAL };

#define savelog_trace(...) savelog_log(SAVELOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define savelog_debug(...) savelog_log(SAVELOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define savelog_info(...)  savelog_log(SAVELOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define savelog_warn(...)  savelog_log(SAVELOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define savelog_error(...) savelog_log(SAVELOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define savelog_fatal(...) savelog_log(SAVELOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

const char* savelog_level_string(int level);
void savelog_set_lock(savelog_LockFn fn, void *udata);
void savelog_set_level(int level);
void savelog_set_quiet(bool enable);
int savelog_add_callback(savelog_LogFn fn, void *udata, int level);
int savelog_add_fp(FILE *fp, int level);

void savelog_log(int level, const char *file, int line, const char *fmt, ...);

#endif
