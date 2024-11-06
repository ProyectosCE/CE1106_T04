// Tamaños constantes
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 450
#define TARGET_FPS 60
#include <stdio.h>
#include "screenHandler.h"
#include "main_menu.h"
#include "nameInput.h"
#include "../game/spectator.h"
#include <raylib.h>



pthread_mutex_t gameStateMutex;
// ============================================================================================================= //

// UPDATE DRAW




// ===================================================================================================
// UPDATE GAME SCREEN

void draw_game_state(GameState *gameState) {
    if (gameState == NULL) {
        fprintf(stderr, "Error: gameState es NULL\n");
        return;
    }

    switch (getCurrentScreen()) {
        case MENU:
            DrawMenu();
        break;

        case NAME_INPUT:
            DrawNameInput(gameState);
        break;

        case GAME:
            draw_game(gameState);
        break;

        case OBSERVER_SELECT: {
            DrawPlayerList();
            break;
        }

        case SPECTATOR:
            draw_game(gameState);
        break;

        default:
            fprintf(stderr, "Advertencia: Pantalla desconocida en draw_game_state\n");
        break;
    }
}


void *draw_thread(void *arg) {
    GameState *gameState = (GameState *)arg;
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Breakout TEC");
    SetTargetFPS(TARGET_FPS);

    while (gameState->running && !WindowShouldClose()) {
        pthread_mutex_lock(&gameStateMutex);
        // Crear una copia local de las partes necesarias del estado del juego
        GameScreen currentScreen = gameState->currentScreen;
        pthread_mutex_unlock(&gameStateMutex);

        // Dibujar utilizando la copia local
        draw_game_state(gameState);
    }

    CloseWindow(); // Cerrar ventana después de terminar el juego

    return NULL;
}
