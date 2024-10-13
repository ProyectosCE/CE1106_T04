//
// Created by jimmy on 12/10/24.
//

#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include <raylib.h>

static const int screenWidth = 800;
static const int screenHeight = 450;

static bool gameOver = false;

void InitGame();         // Initialize game
void UpdateGame();       // Update game (one frame)
void DrawGame();         // Draw game (one frame)
void UnloadGame();       // Unload game
void UpdateDrawFrame();  // Update and Draw (one frame)
void start_game();       // Inicia el juego

#endif //CLIENT_GAME_H
