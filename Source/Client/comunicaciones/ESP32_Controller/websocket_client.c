#include <Python.h>
#include <stdio.h>
#include "websocket_client.h"

// Estructura para pasar argumentos al hilo
typedef struct {
    char esp32_ip[256];
} ThreadArgs;

// Función que se ejecutará en el hilo
void* websocket_thread_function(void* args) {
    ThreadArgs* threadArgs = (ThreadArgs*)args;

    // Llama a la función de WebSocket en un hilo separado
    start_websocket_client(threadArgs->esp32_ip);

    free(threadArgs); // Libera la memoria asignada para los argumentos
    pthread_exit(NULL); // Finaliza el hilo
}

// Función que inicia el hilo para el cliente WebSocket
void start_websocket_client_thread(const char *esp32_ip) {
    pthread_t websocket_thread;
    ThreadArgs* args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
    if (args == NULL) {
        fprintf(stderr, "Error al asignar memoria para los argumentos del hilo.\n");
        return;
    }

    // Copia la dirección IP para pasarla al hilo
    strncpy(args->esp32_ip, esp32_ip, sizeof(args->esp32_ip) - 1);
    args->esp32_ip[sizeof(args->esp32_ip) - 1] = '\0';

    // Crea el hilo
    int result = pthread_create(&websocket_thread, NULL, websocket_thread_function, (void*)args);
    if (result != 0) {
        fprintf(stderr, "Error al crear el hilo: %d\n", result);
        free(args);
        return;
    }

    // Opcional: Espera que el hilo termine si es necesario (puede comentarse si no se necesita)
    // pthread_join(websocket_thread, NULL);
}


void start_websocket_client(const char *esp32_ip) {
    // Inicializa el intérprete de Python
    Py_Initialize();

    // Construye un path relativo o configurable
    const char *relative_path = PYTHON_SCRIPT_PATH;
    char command[256];
    snprintf(command, sizeof(command), "sys.path.append('%s')", relative_path);

    // Agrega el path al `sys.path`
    PyRun_SimpleString("import sys");
    PyRun_SimpleString(command);

    // Importa el módulo de Python
    PyObject *pModule = PyImport_ImportModule("websocket_controller");
    if (pModule != NULL) {
        // Obtén la referencia a la función `start_websocket`
        PyObject *pFunc = PyObject_GetAttrString(pModule, "start_websocket");
        if (PyCallable_Check(pFunc)) {
            // Prepara los argumentos y llama a la función
            PyObject *pArgs = PyTuple_Pack(1, PyUnicode_FromString(esp32_ip));
            PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
            if (pValue != NULL) {
                Py_DECREF(pValue);
            } else {
                PyErr_Print();
                fprintf(stderr, "Error al ejecutar la función Python.\n");
            }
            Py_DECREF(pArgs);
        } else {
            fprintf(stderr, "No se pudo encontrar la función 'start_websocket'.\n");
            PyErr_Print();
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        fprintf(stderr, "Error al importar el módulo Python.\n");
    }

    // Finaliza el intérprete de Python
    Py_Finalize();
}
