#include <Python.h>
#include <stdio.h>
#include "websocket_client.h"

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
