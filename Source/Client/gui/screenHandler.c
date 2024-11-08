
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
#include "screenHandler.h"
#include "main_menu.h"
#include "nameInput.h"
#include "../game/spectator.h"
// BIBLIOTECAS EXTERNAS
#include <stdio.h>
#include <raylib.h>


// Tamaños constantes
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 450
#define TARGET_FPS 60

pthread_mutex_t gameStateMutex;

// ===================================================================================================
// UPDATE GAME SCREEN


/* Function: draw_game_state
   Descripción:
     Renderiza la pantalla correspondiente según el estado actual del juego (`GameState`).
     Cambia entre las pantallas de menú, entrada de nombre, juego, selección de jugador y espectador.

   Params:
     gameState - Puntero a la estructura `GameState` que contiene el estado actual del juego.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `gameState` debe ser un puntero válido antes de llamar a esta función.
     - Las funciones de dibujo correspondientes (`DrawMenu`, `DrawNameInput`, `draw_game`, etc.) deben estar implementadas.

   Example:
     draw_game_state(gameState);
     // Renderiza la pantalla correspondiente según el estado del juego.

   Problems:
     - Problema: Si `gameState` es NULL, no se puede renderizar nada.
       - Solución: Validar el parámetro antes de proceder y registrar un error.
     - Problema: Si `getCurrentScreen` retorna un valor desconocido, puede causar un fallo visual.
       - Solución: Registrar una advertencia y evitar dibujar.

   References:
     - Ninguna referencia externa específica.
*/
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

/* Function: draw_thread
Descripción:
Hilo encargado de manejar el ciclo de renderizado del juego. Inicializa la ventana, establece el FPS,
y renderiza continuamente el estado actual del juego mientras el juego esté en ejecución.

Params:
arg - Puntero a la estructura `GameState` que contiene el estado actual del juego.

Returns:
- void*: Retorna NULL al finalizar el hilo.

Restriction:
- `arg` debe ser un puntero válido a la estructura `GameState`.
- La biblioteca gráfica debe estar correctamente configurada.

Example:
pthread_t thread;
pthread_create(&thread, NULL, draw_thread, gameState);
// Inicia un hilo para manejar el renderizado del juego.

Problems:
- Problema: Si `gameState` es NULL, el hilo no podrá funcionar correctamente.
- Solución: Validar el parámetro antes de proceder.
- Problema: Si ocurre un error con la ventana gráfica (e.g., no se inicializa), puede causar un fallo.
- Solución: Manejar errores al inicializar y registrar mensajes.

References:
- Raylib Documentation: https://www.raylib.com
*/
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
