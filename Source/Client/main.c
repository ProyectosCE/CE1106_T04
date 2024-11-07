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

// Variables globales para el estado de la conexión
bool isConnected = true;
bool isReconnecting = false;
pthread_t reconnectThread;
bool stopReconnect = false;
pthread_mutex_t connectionMutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger el estado de la conexión

// Definir la variable currentScreen
GameScreen currentScreen = MENU;
bool isPlayer = true;
bool isCameraEnabled = false;
ComServer *comServer = NULL;

// Prototipos de funciones
void UpdateMenu();
void DrawMenu();
void *communicationLoop(void *arg);
void DrawReconnectingScreen();
void *reconnectLoop(void *arg);
void startReconnection();
void sendIdentificationMessage();

void onMessageReceived(const char *message) {
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

// Hilo para manejar la reconexión sin bloquear el juego
void *reconnectLoop(void *arg) {
    while (!stopReconnect) {
        pthread_mutex_lock(&connectionMutex);
        if (!SocketServer_isConnected(comServer->socketServer) && !isReconnecting) {
            printf("Perdida de conexión. Pausando el juego e intentando reconectar...\n");
            isReconnecting = true;
            Pause = true;  // Pausar el juego
        }
        pthread_mutex_unlock(&connectionMutex);

        // Intento de reconexión asíncrono
        if (isReconnecting) {
            SocketServer_reconnect(comServer->socketServer);
            sleep(1);

            pthread_mutex_lock(&connectionMutex);
            if (SocketServer_isConnected(comServer->socketServer)) {
                isConnected = true;
                isReconnecting = false;
                printf("Conexión restablecida. Reanudando el juego...\n");

                // Volver a identificarse como jugador
                sendIdentificationMessage();
                Pause = false; // Reanudar el juego
            }
            pthread_mutex_unlock(&connectionMutex);
        }
    }
    return NULL;
}

void sendIdentificationMessage() {
    const char *clientType = "player";
    char *initMessage = JsonProcessor_createClientTypeMessage(clientType);
    if (initMessage != NULL) {
        ComServer_sendMessage(comServer, initMessage);
        free(initMessage);
    } else {
        printf("Error al crear el mensaje de tipo de cliente.\n");
    }
}

void startReconnection() {
    // Crear hilo de reconexión solo si no está activo
    if (!isReconnecting && !stopReconnect) {
        pthread_create(&reconnectThread, NULL, reconnectLoop, NULL);
    }
}

void *communicationLoop(void *arg) {
    ComServer *server = (ComServer *)arg;
    sendIdentificationMessage();  // Enviar mensaje de identificación al iniciar

    while (!stopReconnect) {
        pthread_mutex_lock(&connectionMutex);
        bool currentlyConnected = SocketServer_isConnected(server->socketServer);
        pthread_mutex_unlock(&connectionMutex);

        if (currentlyConnected) {
            ComServer_messageListeningLoop(server);
        } else {
            // Iniciar reconexión
            startReconnection();
            DrawReconnectingScreen();
            sleep(1);
        }
    }
    return NULL;
}

void runMenuLoop() {
    while (currentScreen == MENU) {
        UpdateMenu();
        DrawMenu();
        if (IsKeyPressed(KEY_ENTER)) {
            const char *clientType = isPlayer ? "player" : "spectator";
            comServer = ComServer_create(clientType);
            if (comServer == NULL) {
                printf("Error al crear el servidor de comunicaciones\n");
                return;
            }
            pthread_t communicationThread;
            if (pthread_create(&communicationThread, NULL, communicationLoop, (void *)comServer) != 0) {
                printf("Error al crear el hilo de comunicación\n");
                return;
            }
            if (isPlayer) {
                currentScreen = GAME;
                InitGame();
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
}

void runGameLoop() {
    SetTargetFPS(60);

    while (!WindowShouldClose() && currentScreen == GAME) {
        // Bloquea actualización y muestra pantalla de reconexión si es necesario
        pthread_mutex_lock(&connectionMutex);
        if (isConnected) {
            UpdateDrawFrame();
        } else {
            DrawReconnectingScreen();
        }
        pthread_mutex_unlock(&connectionMutex);
    }
    UnloadGame();
}

void UpdateMenu() {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) {
        isPlayer = !isPlayer;
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT)) {
        isCameraEnabled = !isCameraEnabled;
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

void DrawReconnectingScreen() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Conexión perdida... Pausando el juego", screenWidth / 2 - MeasureText("Conexión perdida... Pausando el juego", 20) / 2, screenHeight / 2 - 40, 20, DARKGRAY);
    DrawText("Intentando reconectar...", screenWidth / 2 - MeasureText("Intentando reconectar...", 20) / 2, screenHeight / 2, 20, DARKGRAY);
    DrawText("Por favor, espere.", screenWidth / 2 - MeasureText("Por favor, espere.", 20) / 2, screenHeight / 2 + 40, 20, GRAY);
    EndDrawing();
}

int main(void) {
    InitWindow(screenWidth, screenHeight, "Breakout TEC Menu");
    runMenuLoop();
    if (currentScreen == GAME) {
        runGameLoop();
    }
    stopReconnect = true;
    pthread_join(reconnectThread, NULL);  // Asegurarse de finalizar el hilo de reconexión
    CloseWindow();
    return 0;
}
