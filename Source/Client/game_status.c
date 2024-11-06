#include "game_status.h"
#include <stdlib.h>

static GameState *gameStateInstance = NULL;
float brickSpacing = 5.0f;

void initGameState() {
    if (gameStateInstance == NULL) {
        gameStateInstance = (GameState *)malloc(sizeof(GameState));
        gameStateInstance->currentScreen = MENU;
        gameStateInstance->isCameraEnabled = false;
        gameStateInstance->isPlayer = true;
        gameStateInstance->lives = 3;  // Inicializar con vidas predeterminadas, por ejemplo
        gameStateInstance->score = 0;
        gameStateInstance->levelsCompleted = 0;
        gameStateInstance->gameOver = false;
        gameStateInstance->running=false;
        gameStateInstance->comunicationRunning=false;
        gameStateInstance->ball_speed_multiplier = 1.0f;
        gameStateInstance->brickSize = (Vector2){ 40, 20 }; // Tamaño predeterminado de los ladrillos


        // Asignar memoria para las bolas y los ladrillos
        gameStateInstance->balls = (Ball *)malloc(MAX_BALLS * sizeof(Ball));
    }
}
// Getters
GameScreen getCurrentScreen() {
    if (gameStateInstance == NULL) initGameState();
    return gameStateInstance->currentScreen;
}

GameState *getGameState() {
    if (gameStateInstance == NULL) initGameState();
    return gameStateInstance;
}

bool isCameraEnabled() {
    if (gameStateInstance == NULL) initGameState();
    return gameStateInstance->isCameraEnabled;
}

bool isPlayer() {
    if (gameStateInstance == NULL) initGameState();
    return gameStateInstance->isPlayer;
}

// Setters
void setCurrentScreen(GameScreen screen) {
    if (gameStateInstance == NULL) initGameState();
    gameStateInstance->currentScreen = screen;
}

void setCameraEnabled(bool enabled) {
    if (gameStateInstance == NULL) initGameState();
    gameStateInstance->isCameraEnabled = enabled;
}

void setPlayer(bool player) {
    if (gameStateInstance == NULL) initGameState();
    gameStateInstance->isPlayer = player;
}