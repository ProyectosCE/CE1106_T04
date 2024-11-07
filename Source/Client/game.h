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

// Estructura para los poderes del bloque
typedef struct BrickPower {
    bool addLife;
    bool addBall;
    bool doubleRacket;
    bool halfRacket;
    bool speedUp;
    bool speedDown;
} BrickPower;

// Estructura del ladrillo, ahora incluye `BrickPower`
typedef struct Brick {
    Vector2 position;
    bool active;
    int points;
    Color color;
    BrickPower power; // Estructura para los poderes
} Brick;

typedef enum {
    MENU,
    GAME,
    RECONNECTING,
    SPECTATOR
} GameScreen;

// Declarar currentScreen como extern para que sea accesible desde otros archivos
extern GameScreen currentScreen;
extern bool isConnected;
extern bool isReconnecting;
extern bool stopReconnect;

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
// Funciones para manejar los poderes
void applyPower(BrickPower power);

// Funciones para poderes específicos
void add_ball(int posX, int posY);
void add_life();
void doubleRacket();
void halfRacket();
void speedUp();
void speedDown();

void processUpdateBrickMessage(cJSON *data);
char* generateGameStateJSON();

#endif //CLIENT_GAME_H
