#ifndef GAME_STATUS_H
#define GAME_STATUS_H
#include <raylib.h>
#include <pthread.h>

#include "comunicaciones/comServer.h"


typedef enum {
    MENU,
    NAME_INPUT,
    OBSERVER_SELECT,
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

// Enumeración para los poderes
typedef enum PowerType {
    NONE,           // Sin poder
    ADD_LIFE,       // Añadir vida
    ADD_BALL,       // Añadir bola
    DOUBLE_RACKET,  // Raqueta doble
    HALF_RACKET,    // Raqueta a la mitad
    SPEED_UP,       // Aumentar velocidad
    SPEED_DOWN      // Reducir velocidad
} PowerType;

// Estructura Brick
typedef struct Brick {
    Vector2 position;  // Posición del ladrillo
    bool active;       // Si el ladrillo está activo
    int points;        // Puntos al destruirlo
    Color color;       // Color del ladrillo
    PowerType power;   // Tipo de poder asociado al ladrillo
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
    bool comunicationRunning;
    ComServer *comServer;
    pthread_t communicationThread;
    pthread_t sendStatusThread;
    pthread_t askForUsersThread;
} GameState;


static const int screenWidth = 800;
static const int screenHeight = 450;
extern float brickSpacing;

// Mutex para sincronizar el acceso a gameState
extern pthread_mutex_t gameStateMutex;


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
