#ifndef GAME_STATUS_H
#define GAME_STATUS_H
#include <raylib.h>
#include <pthread.h>

#include "comunicaciones/comServer.h"


/*
 * Header: Game Data Structures
 * Este archivo contiene la definición de estructuras y enumeraciones fundamentales para representar
 * los elementos principales del juego, como pantallas, ladrillos, bolas, jugadores y el estado general del juego.
 *
 * Typedefs:
 *   - GameScreen: Enumeración que representa las distintas pantallas del juego.
 *   - Ball: Estructura que define las propiedades de una bola en el juego.
 *   - PowerType: Enumeración que representa los distintos poderes disponibles en el juego.
 *   - Brick: Estructura que define las propiedades de un ladrillo en el juego.
 *   - Player: Estructura que define las propiedades de un jugador en el juego.
 *   - GameState: Estructura principal que encapsula el estado general del juego.
 *
 * Enumerations:
 *   - GameScreen:
 *       - MENU: Pantalla del menú principal.
 *       - NAME_INPUT: Pantalla de ingreso del nombre del jugador.
 *       - OBSERVER_SELECT: Pantalla de selección de observador.
 *       - GAME: Pantalla principal del juego.
 *       - SPECTATOR: Pantalla de espectador.
 *       - EXIT: Estado de salida del juego.
 *   - PowerType:
 *       - NONE: Sin poder asociado.
 *       - ADD_LIFE: Añadir vida al jugador.
 *       - ADD_BALL: Añadir una nueva bola al juego.
 *       - DOUBLE_RACKET: Doblar el tamaño de la raqueta.
 *       - HALF_RACKET: Reducir el tamaño de la raqueta a la mitad.
 *       - SPEED_UP: Aumentar la velocidad de las bolas.
 *       - SPEED_DOWN: Reducir la velocidad de las bolas.
 *       - UPDATE_POINTS: Actualizar los puntos del jugador.
 *
 * Macros:
 *   (Ninguna macro definida directamente en esta sección).
 *
 * Functions:
 *   (Ninguna función definida directamente en esta sección).
 *
 * References:
 *   - Ninguna referencia externa específica.
 */
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
    bool isControllerActive;
} GameState;


static const int screenWidth = 800;
static const int screenHeight = 450;
extern float brickSpacing;

// Mutex para sincronizar el acceso a gameState
extern pthread_mutex_t gameStateMutex;

static int levelsCompleted = 0;

/*
 * Header: Game Status
 * Este archivo define las funciones y variables relacionadas con el manejo del estado del juego.
 * Implementa un patrón singleton para garantizar que exista una única instancia del estado global del juego.
 *
 * Typedefs:
 *   - Ninguno.
 *
 * Enumerations:
 *   - Ninguna.
 *
 * Macros:
 *   - GAME_STATUS_H: Define un protector de inclusión para evitar múltiples inclusiones del archivo.
 *
 * Functions:
 *   - initGameState: Inicializa la instancia única del estado del juego.
 *   - getGameState: Devuelve un puntero a la instancia única del estado del juego.
 *   - getCurrentScreen: Obtiene la pantalla actual activa en el juego.
 *   - isCameraEnabled: Verifica si la cámara está habilitada.
 *   - isPlayer: Verifica si el modo de juego actual es el de jugador.
 *   - setCurrentScreen: Establece la pantalla activa en el juego.
 *   - setCameraEnabled: Habilita o deshabilita la cámara.
 *   - setPlayer: Cambia el modo de juego entre jugador y espectador.
 *
 * References:
 *   - Ninguna referencia externa específica.
 */

// Inicializar el estado del juego (singleton)
void initGameState();

GameState *getGameState();
// Getters
GameScreen getCurrentScreen();
bool isCameraEnabled();
bool isPlayer();
bool isControllerActive();

// Setters
void setCurrentScreen(GameScreen screen);
void setCameraEnabled(bool enabled);
void setPlayer(bool player);
void setControllerEnable();

#endif // GAME_STATUS_H
