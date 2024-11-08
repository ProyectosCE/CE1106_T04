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
#include <Python.h>
#include <stdio.h>
#include "websocket_client.h"

// Estructura para pasar argumentos al hilo
typedef struct {
    char esp32_ip[256];
} ThreadArgs;

/* Function: websocket_thread_function
   Esta función es ejecutada en un hilo separado y se encarga de iniciar el cliente WebSocket utilizando la dirección IP proporcionada.

   Params:
   - args: void* - Puntero a una estructura que contiene la dirección IP del ESP32.

   Returns:
   - void* - El retorno del hilo (si aplica, aunque no se usa en este caso).

   Restriction:
   La función debe ser llamada desde un contexto de hilo, ya que utiliza `pthread_exit` para finalizar el hilo.

   Example:
   pthread_create(&thread, NULL, websocket_thread_function, args);
*/
void* websocket_thread_function(void* args) {
    ThreadArgs* threadArgs = (ThreadArgs*)args;

    // Llama a la función de WebSocket en un hilo separado
    start_websocket_client(threadArgs->esp32_ip);

    free(threadArgs); // Libera la memoria asignada para los argumentos
    pthread_exit(NULL); // Finaliza el hilo
}

/* Function: start_websocket_client_thread
   Inicia un nuevo hilo para ejecutar la función del cliente WebSocket, pasando la dirección IP del ESP32.

   Params:
   - esp32_ip: const char* - Dirección IP del ESP32 como cadena de texto.

   Returns:
   - void - No retorna ningún valor.

   Restriction:
   La memoria asignada para los argumentos del hilo debe ser liberada dentro del hilo para evitar fugas de memoria.

   Example:
   start_websocket_client_thread("192.168.0.10");
*/
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

/* Function: start_websocket_client
   Inicializa el intérprete de Python, configura el entorno necesario y llama a la función Python `start_websocket` para iniciar el cliente WebSocket.

   Params:
   - esp32_ip: const char* - Dirección IP del ESP32 que se pasa como argumento a la función Python.

   Returns:
   - void - No retorna ningún valor.

   Restriction:
   Se requiere que la variable `PYTHON_SCRIPT_PATH` esté definida y apunte al directorio del script Python. Además, el módulo `websocket_controller` debe estar disponible en dicho directorio.

   Problems:
   - Error al importar el módulo Python (`websocket_controller`).
   - Error al ejecutar la función Python (`start_websocket`).

   Example:
   start_websocket_client("192.168.0.10");
*/
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
