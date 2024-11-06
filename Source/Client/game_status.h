#ifndef GAME_STATUS_H
#define GAME_STATUS_H
#include <raylib.h>
#include <pthread.h>

#include "comunicaciones/comServer.h"


typedef enum {
    MENU,
    NAME_INPUT,
    GAME,
    SPECTATOR,
    EXIT
} GameScreen;

typedef struct Ball {
    Vector2 position;
    Vector2 speed;
    int radius;
    bool active;
} Ball;

#define MAX_BALLS 5

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

#define LINES_OF_BRICKS         8
#define BRICKS_PER_LINE        8

typedef struct Player {
    Vector2 position;
    Vector2 size;
    int life;
    int score;
    bool doubleRacket;
    bool halfRacket;
} Player;

#define PLAYER_MAX_LIFE         3


typedef struct {
    Player player;
    Ball *balls;
    Brick bricks[LINES_OF_BRICKS][BRICKS_PER_LINE];  // Ahora es una matriz bidimensional
    int lives;
    int score;
    int levelsCompleted;
    bool gameOver;
    float ball_speed_multiplier;
    Vector2 brickSize;
    GameScreen currentScreen;
    bool isCameraEnabled;
    bool isPlayer;
    bool pause;
    bool restart;
    char playerName[50];
    bool running;  // Flag para el estado del juego
    ComServer *comServer;
    pthread_t communicationThread;
    pthread_t sendStatusThread;
    pthread_t gameUpdateThread;
} GameState;


static const int screenWidth = 800;
static const int screenHeight = 450;
extern float brickSpacing;




static bool levelSpeedChanged[LINES_OF_BRICKS / 2] = {false};  // Hay dos líneas por nivel
static int levelsCompleted = 0;


// Inicializar el estado del juego (singleton)
void initGameState();

GameState *getGameState();
// Getters
GameScreen getCurrentScreen();
bool isCameraEnabled();
bool isPlayer();

// Setters
void setCurrentScreen(GameScreen screen);
void setCameraEnabled(bool enabled);
void setPlayer(bool player);

#endif // GAME_STATUS_H
