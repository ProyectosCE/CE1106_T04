//BIBLIOTECAS EXTERNAS
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

//PAQEUTES DEL PROYECTO
#include "comunicaciones/comServer.h"
#include "game.h"
#include "spectator.h"
#include "configuracion/configuracion.h"


typedef enum {
    MENU,
    GAME,
    SPECTATOR
} GameScreen;

GameScreen currentScreen = MENU;
bool isPlayer = true;

void UpdateMenu();
void DrawMenu();
void *gameLoop(void *arg);

void onMessageReceived(const char *message) {
    //TODO: PROCESAR MENSAJES RECIVIDOS
    printf("Respuesta procesada del servidor: %s\n", message);

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
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) {
        isPlayer = !isPlayer;  // Cambia entre jugador y espectador
    }

    if (IsKeyPressed(KEY_ENTER)) {
        ComServer *comServer = ComServer_create();
        if (isPlayer) {
            InitGame();
            currentScreen = GAME;
            ComServer_sendMessage(comServer, "player");
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
    DrawText("Press Enter to Play", screenWidth / 2 - MeasureText("Press Enter to Play", 20) / 2, screenHeight / 2 + 80, 20, DARKGRAY);

    EndDrawing();
}

int main(void) {
    inicializar_configuracion();
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