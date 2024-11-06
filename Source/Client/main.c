#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "screen_handler.h"
#include "game/game_server.h"
volatile bool running; // Flag para controlar la ejecución

// Tamaños constantes
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 450
#define TARGET_FPS 60


// Prototipos de funciones
void *gameLoop(void *arg);

int main(void) {
    pthread_t  gameThread;
    running=true;
    if (pthread_create(&gameThread, NULL, gameLoop, NULL) != 0) {
        fprintf(stderr, "Error al crear el hilo del juego\n");
        running = false; // Detener el hilo de comunicación
        return EXIT_FAILURE;

    }
    // Esperar a que termine el hilo del juego
    pthread_join(gameThread, NULL);
    running = false;

    CloseWindow(); // Cerrar ventana después de terminar el juego
    return EXIT_SUCCESS;
}

// Función para el bucle del juego

void *gameLoop(void *arg) {
    init_game_server();
    GameState *gameState=getGameState();
    // Inicializar ventana
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Breakout TEC");
    SetTargetFPS(TARGET_FPS);
    // Bucle principal del juego
    while (running && !WindowShouldClose()) {
        start_game();
    }
    CloseWindow(); // Cerrar ventana del juego
    // Liberar recursos del juego
    unload_game_server();
    return NULL;
}
