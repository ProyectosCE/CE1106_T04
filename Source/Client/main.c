//BIBLIOTECAS EXTERNAS
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

//PAQEUTES DEL PROYECTO
#include "comunicaciones/comServer.h"
#include "game.h"
#include "spectator.h"


typedef enum {
    MENU,
    GAME,
    SPECTATOR
} GameScreen;

GameScreen currentScreen = MENU;
bool isPlayer = true;
bool isCameraEnabled = false;

void UpdateMenu();
void DrawMenu();
void *gameLoop(void *arg);

void onMessageReceived(const char *message) {
    //TODO: PROCESAR MENSAJES RECIVIDOS
    printf("Respuesta procesada del servidor: %s\n", message);

}


void camera() {
    pid_t pid = fork();
    if (pid == 0) { // Proceso hijo
        // Redirigir la salida de error a error.log
        freopen("error.log", "w", stderr);
        execlp("python3", "python3", "/home/jose/CE1106_T04/Source/computer_vison.py", (char *)NULL);
        // Si execlp falla
        perror("Error al ejecutar el script de Python");
        exit(1); // Salir del hijo si hubo error
    } else if (pid < 0) {
        // Error al hacer fork
        printf("Error al crear un subproceso\n");
    }

}

void *gameLoop(void *arg) {
    InitWindow(screenWidth, screenHeight, "Breakout TEC");

    InitGame();
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        start_game();
    }

    // De-Initialization
    UnloadGame();
    CloseWindow();

    return NULL;
}


void start_game() {
    if (currentScreen == MENU) {
        UpdateMenu();
        DrawMenu();
    } else if (currentScreen == GAME) {
        UpdateDrawFrame();
    } else if (currentScreen == SPECTATOR) {
        // TODO: Implementar el loop del espectador
    }
}

void UpdateMenu() {
    // Navegación por el menú con las flechas
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) {
        isPlayer = !isPlayer;  // Cambia entre jugador y espectador
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT)) {
        isCameraEnabled = !isCameraEnabled;  // Cambia el estado de la cámara
    }

    // Acción al presionar Enter
    if (IsKeyPressed(KEY_ENTER)) {
        ComServer *comServer = ComServer_create();
        if (isPlayer) {
            InitGame();
            currentScreen = GAME;
            ComServer_sendMessage(comServer, "player");

            if (isCameraEnabled) {
                ComServer_sendMessage(comServer, "camera_enabled");
                camera();  // Inicia la cámara si está habilitada
            } else {
                ComServer_sendMessage(comServer, "camera_disabled");
            }
        } else {
            currentScreen = SPECTATOR;
            ComServer_sendMessage(comServer, "spectator");
        }
    }
}

void DrawMenu() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("Choose Mode:", screenWidth / 2 - MeasureText("Choose Mode:", 20) / 2, screenHeight / 2 - 60, 20, DARKGRAY);
    DrawText("1. Player", screenWidth / 2 - MeasureText("1. Player", 20) / 2, screenHeight / 2 - 20, 20, isPlayer ? BLACK : GRAY);
    DrawText("2. Spectator", screenWidth / 2 - MeasureText("2. Spectator", 20) / 2, screenHeight / 2 + 20, 20, !isPlayer ? BLACK : GRAY);
    DrawText("Game Camera: ", screenWidth / 2 - MeasureText("Game Camera: ", 20) / 2, screenHeight / 2 + 60, 20, DARKGRAY);
    DrawText(isCameraEnabled ? "Enabled" : "Disabled", screenWidth / 2 + MeasureText("Game Camera: ", 20) / 2, screenHeight / 2 + 60, 20, isCameraEnabled ? GREEN : RED);
    DrawText("Press Enter to Play", screenWidth / 2 - MeasureText("Press Enter to Play", 20) / 2, screenHeight / 2 + 120, 20, DARKGRAY);

    EndDrawing();
}

int main(void) {
    pthread_t communicationThread, gameThread;

    // Crear la instancia del ComServer
    ComServer *comServer = ComServer_create();

    // Crear el hilo para manejar las comunicaciones con el servidor
    if (pthread_create(&communicationThread, NULL, ComServer_messageListeningLoop, (void *) comServer) != 0) {
        printf("Error al crear el hilo de comunicación\n");


    }
    // Crear hilo para el bucle del juego
    if (pthread_create(&gameThread, NULL, gameLoop, NULL) != 0) {
        printf("Error al crear el hilo del juego\n");
        return 1;
    }

    pthread_join(gameThread, NULL);

    return 0;
}