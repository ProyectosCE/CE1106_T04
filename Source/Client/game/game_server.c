#include "game_server.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "game_logic.h"
#include "game_screen.h"
#include "../game_status.h"
#include "../gui/main_menu.h"
#include "spectator.h"
#include "Objects/ball.h"
#include "Objects/brick.h"
#include "Objects/player.h"
#include "powerHandler.h"


pthread_t sendStateThread;
pthread_t askForUserThread;

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

}


void *update_game_thread(void *arg) {
    GameState *gameState = (GameState *)arg;
    update_game(gameState);

    return NULL;
}

void *draw_game_thread(void *arg) {
    GameState *gameState = (GameState *)arg;
    while (gameState->running) {
        draw_game(gameState);
    }
    return NULL;
}

void *send_game_state_thread(void *arg) {
    GameState *gameState = (GameState *)arg;
    while (gameState->running) {
        sendGameState(gameState);
    }  // Send the current game state
    return NULL;
}

void *askForPlayerList(void *arg) {
    GameState *gameState = (GameState *)arg;
    ComServer *comServer=ComServer_create();
    while (gameState->currentScreen==OBSERVER_SELECT) {
        ComServer_observerGetlist(comServer);
        sleep(5);
    }  // Send the current game state
    return NULL;
}

void DrawMenuInput(GameState *gameState) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Enter your name:", 100, 100, 20, DARKGRAY);
    DrawText(gameState->playerName, 100, 130, 20, BLACK);
    DrawText("Press Enter to Play", 100, 160, 20, DARKGRAY);
    EndDrawing();
}

void updateMenuInput(GameState *game_state) {
    if (IsKeyPressed(KEY_ENTER)) {
        game_state->comServer = ComServer_create();
        ComServer_sendPlayerName(game_state->comServer, game_state->playerName);
        init_game_server();
        setCurrentScreen(GAME);
    } else if (IsKeyPressed(KEY_BACKSPACE) && strlen(game_state->playerName) > 0) {
        game_state->playerName[strlen(game_state->playerName) - 1] = '\0'; // Elimina el último carácter
    } else {
        for (int key = KEY_A; key <= KEY_Z; key++) {
            if (IsKeyPressed(key) && strlen(game_state->playerName) < sizeof(game_state->playerName) - 1) {
                game_state->playerName[strlen(game_state->playerName)] = (char)(key + 'A' - KEY_A); // Agrega letra
                game_state->playerName[strlen(game_state->playerName) + 1] = '\0'; // Termina la cadena
            }
        }
    }
}


void gameServerCallback(const char *recibido) {
    printf(recibido);
}


void start_game() {
    GameState *gameState = getGameState();

    if (getCurrentScreen() == MENU) {
        UpdateMenu();
        DrawMenu();
}
    else if (getCurrentScreen() == NAME_INPUT) {
        updateMenuInput(gameState);
        DrawMenuInput(gameState);

    } else if (getCurrentScreen() == GAME) {
        if (!gameState->running) {
            gameState->running = true;

            // Crear instancia del servidor de comunicaciones

            if (gameState->comServer == NULL) {
                fprintf(stderr, "Error al crear ComServer\n");
                gameState->running = false;
            }
            ComServer_registerCallback(gameState->comServer,gameServerCallback);
            // Crear el hilo de comunicaciones
            if (pthread_create(&gameState->communicationThread, NULL, ComServer_messageListeningLoop, (void *)gameState->comServer) != 0) {
                fprintf(stderr, "Error al crear el hilo de comunicación\n");
                ComServer_destroy(gameState->comServer);
                gameState->running = false;
            }

            // Crear el hilo de comunicaciones
            if (pthread_create(&gameState->sendStatusThread, NULL, send_game_state_thread, (void *)gameState) != 0) {
                fprintf(stderr, "Error al crear el hilo de comunicación\n");
                ComServer_destroy(gameState->comServer);
                gameState->running = false;
            }
        }

        // Crear los hilos para manejar las tareas de actualización, dibujo y envío
        update_game(gameState);
        draw_game(gameState);

        // Manejar la condición de reinicio del juego
        if (gameState->restart) {
            init_game_server();
        }
    }
    else if (getCurrentScreen() == OBSERVER_SELECT) {

        // Crear el hilo de comunicaciones
        ComServer *comServer = ComServer_create();
        // Print the boolean value
        if (!gameState->running) {
            // Print the boolean value

            printf("Game is %s\n", gameState->running ? "running" : "not running");
            gameState->running = true;

            ComServer_registerCallback(comServer,espectadorGetList);
            gameState->comServer=comServer;

            if (pthread_create(&gameState->communicationThread, NULL, ComServer_messageListeningLoop, (void *)gameState->comServer) != 0) {
                fprintf(stderr, "Error al crear el hilo de comunicación\n");
                ComServer_destroy(gameState->comServer);
                gameState->running = false;
            }

            if (pthread_create(&gameState->askForUsersThread, NULL, askForPlayerList, (void *)gameState) != 0) {
                fprintf(stderr, "Error al crear el hilo de comunicación\n");
                ComServer_destroy(gameState->comServer);
                gameState->running = false;
            }
        }
        PlayerList *playerLista = GetPlayerListInstance();
        DrawPlayerList(playerLista);
        UpdatePlayerList(playerLista);

    }

    else if (getCurrentScreen() == SPECTATOR) {
        if (!gameState->running) {
            gameState->running=true;
            ComServer *comServer = ComServer_create();
            init_game_server();
            ComServer_registerCallback(comServer,espectadorUpdateGame);
        }
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