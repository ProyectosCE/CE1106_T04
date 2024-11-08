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
#ifndef SCREEN_HANDLER_H
#define SCREEN_HANDLER_H

#include <pthread.h>
#include "../game_status.h"  // Asegúrate de que `GameState` y `GameScreen` estén definidos aquí
#include <raylib.h>

// Constantes para la ventana
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 450
#define TARGET_FPS 60



// Funciones de dibujo
void draw_game_state(GameState *gameState);
void *draw_thread(void *arg);

#endif // SCREEN_HANDLER_H
