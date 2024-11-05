//
// Created by jimmy on 12/10/24.
//

#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include <raylib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <cjson/cJSON.h>



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

typedef struct Brick {
    Vector2 position;
    bool active;
    int points;
    Color color;
    bool add_life;
    bool add_ball;
    bool add_doubleRacket;
    bool add_halfRacket;
    bool speedUp;
    bool speedDown;
} Brick;

#define PLAYER_MAX_LIFE         3
#define MAX_BALLS              5
#define LINES_OF_BRICKS         8
#define BRICKS_PER_LINE        8

static const int screenWidth = 800;
static const int screenHeight = 450;

static bool gameOver = false;
static bool Pause = false;

static Player player = { 0 };
static Ball balls[MAX_BALLS] = { 0 };
static Brick brick[LINES_OF_BRICKS][BRICKS_PER_LINE] = { 0 };
static Vector2 brickSize = { 0 };
static bool levelSpeedChanged[LINES_OF_BRICKS / 2] = {false};  // Hay dos líneas por nivel
static int levelsCompleted = 0;

void InitGame();         // Initialize game
void UpdateGame();       // Update game (one frame)
void DrawGame();         // Draw game (one frame)
void UnloadGame();       // Unload game
void UpdateDrawFrame();  // Update and Draw (one frame)
void start_game();       // Inicia el juego

void check_brick(int i, int j);

void update_brick_score(int level, int new_points);
void update_player_score(int brickx, int bricky);

void update_brick_ball(int i, int j);
void add_ball(int i, int j);

void update_brick_life(int i, int j);
void add_life();

void update_brick_doubleRacket(int i, int j);
void doubleRacket();

void update_brick_halfRacket(int i, int j);
void halfRacket();

void update_brick_speedUp(int i, int j);
void speedUp();

void update_brick_speedDown(int i, int j);
void speedDown();

char* generateGameStateJSON();

#endif //CLIENT_GAME_H
