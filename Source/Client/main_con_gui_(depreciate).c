/* #include <stdio.h>
#include <unistd.h>
#include "connection.h"
#include "game.h"
#include "spectator.h"
#include <pthread.h>

typedef enum {
    MENU,
    GAME,
    SPECTATOR
} GameScreen;

GameScreen currentScreen = MENU;
bool isPlayer = true;

void UpdateMenu();
void DrawMenu();
void *serverCommunication(void *arg);
void *gameLoop(void *arg);

int main(void) {
    pthread_t serverThread, gameThread;

    // Crear hilo para la comunicación con el servidor
    if (pthread_create(&serverThread, NULL, serverCommunication, NULL) != 0) {
        printf("Error al crear el hilo del servidor\n");
        return 1;
    }

    // Crear hilo para el bucle del juego
    if (pthread_create(&gameThread, NULL, gameLoop, NULL) != 0) {
        printf("Error al crear el hilo del juego\n");
        return 1;
    }

    pthread_join(gameThread, NULL);

    return 0;
}

void *serverCommunication(void *arg) {
    start_server();  // Inicia la conexión con el servidor

    // Bucle para manejar la comunicación constante con el servidor
    while (1) {
        if (readServerMessage() <= 0) {
            exit(1);
        }
    }

    return NULL;
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
        if (isPlayer) {
            InitGame();
            currentScreen = GAME;
            sendClientType("player");
        } else {
            currentScreen = SPECTATOR;
            sendClientType("spectator");

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
*/