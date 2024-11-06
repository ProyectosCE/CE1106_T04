#include "game_server.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../logs/saveLog.h"
#include <errno.h>


#include "game_logic.h"
#include "../gui/main_menu.h"
#include "spectator.h"
#include "Objects/ball.h"
#include "Objects/brick.h"
#include "Objects/player.h"
#include "powerHandler.h"

#include "../gui/screenHandler.h"




pthread_t askForUserThread;
// Declarar el mutex global

void init_game_server() {

    GameState *gameStatus = getGameState();
    init_player(&gameStatus->player, gameStatus->playerMaxLife);
    init_balls(gameStatus->balls);

    // Inicializar los ladrillos
    brickSize = (Vector2){ screenWidth/gameStatus->bricksPerLine, screenHeight/(gameStatus->linesOfBricks*2) };
    gameStatus->brickSize = brickSize;
    init_bricks(gameStatus,gameStatus->brickSize);

    initPowerHandler();

    gameStatus->restart=false;
    gameStatus->pause=false;
    gameStatus->winner=false;
    gameStatus->gameOver = false;
    gameStatus->levelsCompleted=0;

}


void gameServerCallback(const char *recibido) {
    process_brick_update(recibido);
}

void updateNameInput(GameState *game_state) {
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


// ================================================================================================================= //
// COMUNICACION THREAD
void initialize_game_communication(GameState *gameState, void (*callback)(void *)) {
    if (gameState->comServer == NULL) {
        gameState->comServer = ComServer_create();
        if (gameState->comServer == NULL) {
            fprintf(stderr, "Error al crear ComServer\n");
            gameState->running = false;
            return;
        }
        ComServer_registerCallback(gameState->comServer, callback);
    }

    if (pthread_create(&gameState->communicationThread, NULL, ComServer_messageListeningLoop, (void *)gameState->comServer) != 0) {
        fprintf(stderr, "Error al crear el hilo de comunicación\n");
        ComServer_destroy(gameState->comServer);
        gameState->running = false;
    }
}

// ============================================================================================================== //
// UPDATE GAME STATE
void update_game_state(GameState *gameState) {
    switch (getCurrentScreen()) {
        case MENU:
            UpdateMenu();
            break;

        case NAME_INPUT:
            updateNameInput(gameState);
            break;

        case GAME:
            if (!gameState->comunicationRunning) {

                gameState->comunicationRunning=true;

                printf("COMUNICACIONES INICIADAS EN GAMEE!");

                gameState->comServer=NULL;

                // Inicializar comunicación del juego
                initialize_game_communication(gameState, gameServerCallback);

                // Crear el hilo para enviar estados del juego
                if (pthread_create(&gameState->sendStatusThread, NULL, send_game_state_thread, (void *)gameState) != 0) {
                    fprintf(stderr, "Error al crear el hilo de envío de estado\n");
                    ComServer_destroy(gameState->comServer);
                    gameState->running = false;
                    gameState->comunicationRunning=false;
                    return;
                }
            }

            update_game(gameState);

            // Reinicio del juego
            if (gameState->restart) {
                printf("REINICIANDOOOOO\n\n\n\n");
                init_game_server();
            }
            break;

        case OBSERVER_SELECT: {
            if (!gameState->comunicationRunning) {

                // Inicializar comunicación del observador
                initialize_game_communication(gameState, espectadorGetList);

                init_game_server();
                gameState->running=true;
                gameState->comunicationRunning=true;

                // Crear el hilo para solicitar la lista de jugadores
                if (pthread_create(&gameState->askForUsersThread, NULL, askForPlayerList, (void *)gameState) != 0) {
                    fprintf(stderr, "Error al crear el hilo de solicitud de jugadores\n");
                    ComServer_destroy(gameState->comServer);
                    gameState->comunicationRunning=false;
                    gameState->running = false;
                    return;
                }
            }

            PlayerList *playerLista = GetPlayerListInstance();
            if (playerLista != NULL) {
                UpdatePlayerList(playerLista);
            }
            break;
        }

        case SPECTATOR:
            if (!gameState->comunicationRunning) {
                init_game_server();
                gameState->running=true;
                gameState->comunicationRunning = true;
                gameState->comServer=NULL;

                // Inicializar comunicación del espectador
                initialize_game_communication(gameState, espectadorUpdateGame);
            }
            break;

        default:
            break;
    }

    // Verificar si el jugador perdió todas las vidas
    if (gameState->player.life <= 0) {
        gameState->gameOver = true;
    }
}



// ============================================================================================================= //

// UPDATE THREAD

void *update_thread(void *arg) {
    GameState *gameState = (GameState *)arg;

    while (gameState->running) {
        pthread_mutex_lock(&gameStateMutex);
        // Actualizar el estado del juego basado en la pantalla actual
        update_game_state(gameState);
        pthread_mutex_unlock(&gameStateMutex);

        // Limitar la velocidad de actualización (~60 FPS)
        usleep(16000);
    }

    return NULL;
}


// ============================================================================================================= //

// START GAME

void start_game() {
    GameState *gameState = getGameState();
    pthread_t updateThread, drawThread;

    // Inicializar el mutex
    if (pthread_mutex_init(&gameStateMutex, NULL) != 0) {
        fprintf(stderr, "Error al inicializar el mutex\n");
        return;
    }

    // Establecer el estado inicial
    gameState->running = true;

    // Crear e iniciar el thread de actualización
    if (pthread_create(&updateThread, NULL, update_thread, (void *)gameState) != 0) {
        fprintf(stderr, "Error al crear el thread de actualización\n");
        gameState->running = false;
        return;
    }

    // Crear e iniciar el thread de dibujo
    if (pthread_create(&drawThread, NULL, draw_thread, (void *)gameState) != 0) {
        fprintf(stderr, "Error al crear el thread de dibujo\n");
        gameState->running = false;
        return;
    }

    // Esperar a que los threads terminen (opcional)
    pthread_join(updateThread, NULL);
    pthread_join(drawThread, NULL);

    // Destruir el mutex al finalizar
    pthread_mutex_destroy(&gameStateMutex);
}



/// VARAS A ELIMINAR
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