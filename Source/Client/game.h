//
// Created by jimmy on 12/10/24.
//

#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include <raylib.h>

typedef struct Player {
    Vector2 position;
    Vector2 size;
    int life;
    int score;
    bool doubleRacket;
    bool halfRacket;
} Player;

typedef struct Ball {
    Vector2 position;
    Vector2 speed;
    int radius;
    bool active;
} Ball;

#define PLAYER_MAX_LIFE         3
#define MAX_BALLS              20

static const int screenWidth = 800;
static const int screenHeight = 450;

static bool gameOver = false;
static bool Pause = false;

static Player player = { 0 };
static Ball balls[MAX_BALLS] = { 0 };

void InitGame();         // Initialize game
void UpdateGame();       // Update game (one frame)
void DrawGame();         // Draw game (one frame)
void UnloadGame();       // Unload game
void UpdateDrawFrame();  // Update and Draw (one frame)
void start_game();       // Inicia el juego

#endif //CLIENT_GAME_H
