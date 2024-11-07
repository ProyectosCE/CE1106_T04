// BIBLIOTECAS EXTERNAS
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

// PAQUETES DEL PROYECTO
#include "comunicaciones/comServer.h"
#include "comunicaciones/jsonProcessor.h"
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
ComServer *comServer = NULL;

// Prototipos de funciones
void UpdateMenu();
void DrawMenu();
void *gameLoop(void *arg);
void *communicationLoop(void *arg);

void onMessageReceived(const char *message) {
    // TODO: PROCESAR MENSAJES RECIBIDOS
    printf("Respuesta procesada del servidor: %s\n", message);
}

void camera() {
    pid_t pid = fork();
    if (pid == 0) { // Proceso hijo
        freopen("error.log", "w", stderr);
        execlp("python3", "python3", "/home/jose/CE1106_T04/Source/computer_vison.py", (char *)NULL);
        perror("Error al ejecutar el script de Python");
        exit(1); // Salir del hijo si hubo error
    } else if (pid < 0) {
        printf("Error al crear un subproceso\n");
    }
}

void *gameLoop(void *arg) {
    InitWindow(screenWidth, screenHeight, "Breakout TEC");
    InitGame();
    SetTargetFPS(60);
    printf("juego iniciado");

    while (!WindowShouldClose()) {
        start_game();
    }
    printf("juego iniciado");
    sleep(20);
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

    if (IsKeyPressed(KEY_ENTER)) {
        const char *clientType = isPlayer ? "player" : "spectator";

        // Crear la instancia de ComServer con el tipo de cliente seleccionado
        comServer = ComServer_create(clientType);
        if (comServer == NULL) {
            printf("Error al crear el servidor de comunicaciones\n");
            return;
        }

        // Crear el hilo de comunicación
        pthread_t communicationThread;
        if (pthread_create(&communicationThread, NULL, communicationLoop, (void *)comServer) != 0) {
            printf("Error al crear el hilo de comunicación\n");
            return;
        }

        // Cambiar la pantalla según el modo seleccionado
        if (isPlayer) {
            //InitGame();
            currentScreen = GAME;

            // Enviar el estado de la cámara
            if (isCameraEnabled) {
                ComServer_sendMessage(comServer, "camera_enabled");
                camera();
            } else {
                ComServer_sendMessage(comServer, "camera_disabled");
            }
        } else {
            currentScreen = SPECTATOR;
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

void *communicationLoop(void *arg) {
    ComServer *server = (ComServer *)arg;

    // Crear y enviar el mensaje inicial de tipo de cliente
    const char *clientType = isPlayer ? "player" : "spectator";
    char *initMessage = JsonProcessor_createClientTypeMessage(clientType);
    if (initMessage != NULL) {
        ComServer_sendMessage(server, initMessage);
        free(initMessage);
    } else {
        printf("Error al crear el mensaje de tipo de cliente.\n");
    }

    // Bucle de recepción de mensajes del servidor
    ComServer_messageListeningLoop(server);

    return NULL;
}

int main(void) {
    pthread_t gameThread;

    // Crear el hilo del bucle del juego
    if (pthread_create(&gameThread, NULL, gameLoop, NULL) != 0) {
        printf("Error al crear el hilo del juego\n");
        return 1;
    }

    // Esperar a que el hilo del juego finalice
    pthread_join(gameThread, NULL);

    return 0;
}
