#include "game_server.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "game_logic.h"
#include "game_screen.h"
#include "../game_status.h"
#include "../gui/main_menu.h"
#include "spectator.h"
#include "Objects/ball.h"
#include "Objects/brick.h"
#include "Objects/player.h"
#include "powerHandler.h"


void init_game_server() {
    GameState *gameStatus = getGameState();
    init_player(&gameStatus->player);
    init_balls(gameStatus->balls);

    // Inicializar los ladrillos
    brickSize = (Vector2){ screenWidth/BRICKS_PER_LINE, screenHeight/(LINES_OF_BRICKS*2) };
    gameStatus->brickSize = brickSize;
    init_bricks(gameStatus,gameStatus->brickSize);

    initPowerHandler();

    gameStatus->restart=false;
    gameStatus->pause=false;
    gameStatus->gameOver = false;
}

void unload_game_server() {
    // Liberación de recursos si fuera necesario
}

void start_game() {
    GameState *gameState = getGameState();
    if (getCurrentScreen() == MENU) {
        UpdateMenu();
        DrawMenu();
    } else if (getCurrentScreen() == GAME) {
        update_game(gameState);
        draw_game(gameState);
        if /*gameState->gameOver*/(gameState->restart) {
            init_game_server();
        }
    } else if (getCurrentScreen() == SPECTATOR) {
        // Lógica para el espectador
        // Recibir el estado del juego

        const char *filename = "../gameState.json";
        //char *jsonStr = readJSONFile(filename);
        /*if (jsonStr == NULL) {
            printf("Error al leer el archivo JSON.\n");
            return;
        }
        */
        //parseGameState();
        //free(jsonStr); // Liberar la memoria del JSON leído

        draw_game(gameState);
    }

    if (gameState->player.life <= 0) {
        gameState->gameOver = true;
    }
}


void GameServer_handleMessage(const char *message) {
    if (strcmp(message, "start") == 0) {
        init_game_server();
        setCurrentScreen(GAME);
    } else if (strcmp(message, "stop") == 0) {
        GameState *gameState = getGameState();
        gameState->gameOver = true;
        setCurrentScreen(MENU);
    } else {
        printf("Comando desconocido: %s\n", message);
    }
}