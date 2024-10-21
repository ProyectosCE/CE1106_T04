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
#include "saveLog.h"
#include <sys/stat.h>  // Para crear la carpeta

#define MAX_CALLBACKS 32
#define LOG_DIR "logs"
#define LOG_FILE "logs/project.log"

typedef struct {
    savelog_LogFn fn;
    void *udata;
    int level;
} Callback;

static struct {
    void *udata;
    savelog_LockFn lock;
    int level;
    bool quiet;
    Callback callbacks[MAX_CALLBACKS];
    FILE *log_file;  // Archivo para el log
} L;


static const char *level_strings[] = {
        "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

#ifdef LOG_USE_COLOR
static const char *level_colors[] = {
  "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};
#endif

// Crear la carpeta logs si no existe
static void create_log_directory() {
    struct stat st = {0};
    if (stat(LOG_DIR, &st) == -1) {
        mkdir(LOG_DIR, 0700);
    }
}

// Abrir el archivo de log
static void open_log_file() {
    create_log_directory();
    L.log_file = fopen(LOG_FILE, "a");
    if (!L.log_file) {
        fprintf(stderr, "Error al abrir el archivo de log %s\n", LOG_FILE);
        return;
    }
}

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


static void lock(void)   {
    if (L.lock) { L.lock(true, L.udata); }
}


static void unlock(void) {
    if (L.lock) { L.lock(false, L.udata); }
}


const char* savelog_level_string(int level) {
    return level_strings[level];
}


void savelog_set_lock(savelog_LockFn fn, void *udata) {
    L.lock = fn;
    L.udata = udata;
}


void savelog_set_level(int level) {
    L.level = level;
}


void savelog_set_quiet(bool enable) {
    L.quiet = enable;
}


int savelog_add_callback(savelog_LogFn fn, void *udata, int level) {
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        if (!L.callbacks[i].fn) {
            L.callbacks[i] = (Callback) { fn, udata, level };
            return 0;
        }
    }
    return -1;
}


int savelog_add_fp(FILE *fp, int level) {
    return savelog_add_callback(file_callback, fp, level);
}


static void init_event(savelog_Event *ev, void *udata) {
    if (!ev->time) {
        time_t t = time(NULL);
        ev->time = localtime(&t);
    }
    ev->udata = udata;
}


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