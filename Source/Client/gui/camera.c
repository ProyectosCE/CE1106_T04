#include "camera.h"
#include "../game_status.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void start_camera() {

    if (isCameraEnabled()) {
        pid_t pid = fork();
        if (pid == 0) { // Proceso hijo
            freopen("error.log", "w", stderr);
            execlp("python3", "python3", "/home/jose/CE1106_T04/Source/computer_vison.py", (char *)NULL);
            perror("Error al ejecutar el script de Python");
            exit(1);
        } else if (pid < 0) {
            printf("Error al crear un subproceso\n");
        }
    }
}
