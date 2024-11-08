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

#include "camera.h"
#include "../game_status.h"

// BIBLIOTECAS EXTERNAS
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


/* Function: start_camera
Descripción:
Inicia la cámara llamando a un script de Python (`computer_vision.py`) en un proceso hijo.
Si el script falla al ejecutarse, el error se registra en un archivo `error.log`.

Params:
(Ninguno)

Returns:
- void: No retorna valores.

Restriction:
- Se asume que el script `computer_vision.py` está ubicado correctamente en la ruta relativa `../../computer_vision.py`.
- El script debe ser ejecutable en el entorno del sistema operativo.
- La cámara debe estar habilitada (`isCameraEnabled` retorna `true`).

Example:
start_camera();
// Inicia el proceso para manejar la cámara mediante un script de Python.

Problems:
- Problema: Si `fork` falla, no se podrá crear el proceso hijo.
- Solución: Registrar un mensaje de error y evitar operaciones adicionales.
- Problema: Si `execlp` falla, el proceso hijo no podrá ejecutar el script.
- Solución: Registrar el error utilizando `perror` y finalizar el proceso hijo.

References:
- Linux fork(2) man page: https://man7.org/linux/man-pages/man2/fork.2.html
- Linux exec(3) man page: https://man7.org/linux/man-pages/man3/exec.3.html
*/
void start_camera() {

    if (isCameraEnabled()) {
        pid_t pid = fork();
        if (pid == 0) { // Proceso hijo
            freopen("error.log", "w", stderr);
            execlp("python3", "python3", "../../computer_vison.py", (char *)NULL);
            perror("Error al ejecutar el script de Python");
            exit(1);
        } else if (pid < 0) {
            printf("Error al crear un subproceso\n");
        }
    }
}
