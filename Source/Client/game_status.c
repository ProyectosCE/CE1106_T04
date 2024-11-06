#include "game_status.h"

#include <stdio.h>
#include <stdlib.h>
#include "configuracion/configuracion.h"

static GameState *gameStateInstance = NULL;
float brickSpacing = 5.0f;



void initializeLevelSpeedChanged(GameState *gameStateInstance) {
    // Determinar el número de niveles (suponiendo 2 líneas por nivel)
    gameStateInstance->levels = gameStateInstance->linesOfBricks / 2;

    // Asignar memoria dinámica para levelSpeedChanged
    gameStateInstance->levelSpeedChanged = (bool *)malloc(gameStateInstance->levels * sizeof(bool));
    if (!gameStateInstance->levelSpeedChanged) {
        perror("Error al asignar memoria para levelSpeedChanged");
        exit(EXIT_FAILURE);
    }

    // Inicializar los valores a false
    for (int i = 0; i < gameStateInstance->levels; i++) {
        gameStateInstance->levelSpeedChanged[i] = false;
    }
}



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

        //Inicializar datos from INI
        gameStateInstance->maxBalls= get_config_int("game.maxBalls");
        gameStateInstance->bricksPerLine= get_config_int("game.bricksPerLine");
        gameStateInstance->linesOfBricks= get_config_int("game.linesOfBricks");
        gameStateInstance->playerMaxLife= get_config_int("game.playerMaxLife");
        initializeLevelSpeedChanged(gameStateInstance);



        // Asignar memoria para las bolas
        gameStateInstance->balls = (Ball *)malloc(gameStateInstance->maxBalls * sizeof(Ball));
        if (!gameStateInstance->balls) {
            perror("Error al asignar memoria para las bolas");
            exit(EXIT_FAILURE);
        }

        // Asignar memoria para los ladrillos
        gameStateInstance->bricks = (Brick **)malloc(gameStateInstance->linesOfBricks * sizeof(Brick *));
        if (!gameStateInstance->bricks) {
            perror("Error al asignar memoria para las filas de ladrillos");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < gameStateInstance->linesOfBricks; i++) {
            gameStateInstance->bricks[i] = (Brick *)malloc(gameStateInstance->bricksPerLine * sizeof(Brick));
            if (!gameStateInstance->bricks[i]) {
                perror("Error al asignar memoria para los ladrillos por fila");
                exit(EXIT_FAILURE);
            }
        }
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