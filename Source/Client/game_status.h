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


// Enumeración para los poderes
typedef enum PowerType {
    NONE,           // Sin poder
    ADD_LIFE,       // Añadir vida
    ADD_BALL,       // Añadir bola
    DOUBLE_RACKET,  // Raqueta doble
    HALF_RACKET,    // Raqueta a la mitad
    SPEED_UP,       // Aumentar velocidad
    SPEED_DOWN,      // Reducir velocidad
    UPDATE_POINTS
} PowerType;

// Estructura Brick
typedef struct Brick {
    Vector2 position;  // Posición del ladrillo
    bool active;       // Si el ladrillo está activo
    int points;        // Puntos al destruirlo
    Color color;       // Color del ladrillo
    PowerType power;   // Tipo de poder asociado al ladrillo
} Brick;


typedef struct Player {
    Vector2 position;
    Vector2 size;
    int life;
    int score;
    bool doubleRacket;
    bool halfRacket;
} Player;

typedef struct {
    Player player;
    Ball *balls;
    Brick **bricks;
    int linesOfBricks;
    int maxBalls;
    int bricksPerLine;
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
    int playerMaxLife;
    bool *levelSpeedChanged;
    int levels;
    bool winner;
    bool bolaLanzada;
} GameState;


static const int screenWidth = 800;
static const int screenHeight = 450;
extern float brickSpacing;

// Mutex para sincronizar el acceso a gameState
extern pthread_mutex_t gameStateMutex;

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
