#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "comunicaciones/comServer.h"
#include "gui/main_menu.h"
#include "game/game_server.h"

// Tamaños constantes
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 450
#define TARGET_FPS 60

// Estructura para almacenar la información del servidor de juego y la comunicación
typedef struct {
    ComServer *comServer;
    volatile bool running; // Flag para controlar la ejecución
} ServerData;

// Prototipos de funciones
void *gameLoop(void *arg);
void onMessageReceived(const char *message);

int main(void) {
    pthread_t communicationThread, gameThread;
    ServerData serverData;

    // Inicializar el flag de ejecución
    serverData.running = true;

    // Crear instancia de ComServer
    serverData.comServer = ComServer_create();
    if (serverData.comServer == NULL) {
        fprintf(stderr, "Error al crear ComServer\n");
        return EXIT_FAILURE;
    }

    // Configurar callback para manejar mensajes recibidos
    ComServer_registerCallback(serverData.comServer, onMessageReceived);



    // Si el jugador ha elegido un modo de juego, iniciar los hilos

    // Crear hilo para el bucle de comunicaciones
    if (pthread_create(&communicationThread, NULL, ComServer_messageListeningLoop, (void *)serverData.comServer) != 0) {
        fprintf(stderr, "Error al crear el hilo de comunicación\n");
        ComServer_destroy(serverData.comServer);
        return EXIT_FAILURE;
    }

    // Crear hilo para el bucle del juego
    if (pthread_create(&gameThread, NULL, gameLoop, (void *)&serverData) != 0) {
        fprintf(stderr, "Error al crear el hilo del juego\n");
        serverData.running = false; // Detener el hilo de comunicación
        ComServer_destroy(serverData.comServer);
        pthread_cancel(communicationThread);
        return EXIT_FAILURE;
    }

    // Esperar a que termine el hilo del juego
    pthread_join(gameThread, NULL);

    // Detener el hilo de comunicación de manera controlada
    serverData.running = false;
    pthread_cancel(communicationThread);
    pthread_join(communicationThread, NULL);

    // Limpiar recursos
    ComServer_destroy(serverData.comServer);



    CloseWindow(); // Cerrar ventana después de terminar el juego

    return EXIT_SUCCESS;
}

// Función de callback para manejar mensajes recibidos
void onMessageReceived(const char *message) {
    GameServer_handleMessage(message); // Pasar mensaje a GameServer
}

// Función para el bucle del juego
void *gameLoop(void *arg) {
    ServerData *serverData = (ServerData *)arg;

    // Inicializar ventana
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Breakout TEC");
    SetTargetFPS(TARGET_FPS);



    // Bucle principal del juego
    while (serverData->running && !WindowShouldClose()) {
        start_game();
    }

    //UnloadGame();
    CloseWindow();

    // Liberar recursos del juego
    unload_game_server();

    return NULL;
}
