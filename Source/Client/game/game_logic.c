#include "game_logic.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cjson/cJSON.h>

#include "raylib.h"
#include "Objects/player.h"
#include "Objects/ball.h"

#include "../comunicaciones/comServer.h"

#include "collision_handler.h"
#include "powerHandler.h"

void update_game(GameState *gameState) {
    if (IsKeyPressed(KEY_P)) gameState->pause = !gameState->pause;

    if (!gameState->pause) {

        update_player_movement(&gameState->player, screenWidth);
        update_ball_positions(&gameState->player, gameState->balls);
        handle_collisions(gameState);

        if (noBallsActive(gameState->balls, gameState->maxBalls) && gameState->bolaLanzada) {
            gameState->player.life --;
            gameState->bolaLanzada=false;
        }

        // Verificar si el jugador pierde todas las vidas
        if (gameState->player.life <= 0) {
            gameState->gameOver = true;  // Cambiar al menú principal en caso de Game Over
        }

        // Verificar si ya no hay bloques activos
        if (bloquesEliminados(gameState)) {
            gameState->winner=true;
        }

    }
    if (gameState->gameOver || gameState->winner) {
        if (IsKeyPressed(KEY_ENTER)) gameState->restart=true;
    }

}

void *send_game_state_thread(void *arg) {
    GameState *gameState = (GameState *)arg;

    if (gameState == NULL) {
        fprintf(stderr, "Error: gameState es NULL\n");
        return NULL;
    }

    while (true) {
        // Crear una copia local del estado del juego
        GameState localState;

        pthread_mutex_lock(&gameStateMutex);
        bool isRunning = gameState->running;
        if (isRunning) {
            localState = *gameState; // Copiar todo el estado del juego
        }
        pthread_mutex_unlock(&gameStateMutex);

        if (!isRunning) {
            break;
        }

        // Enviar el estado del juego usando la copia local
        sendGameState(&localState);

        // Pausar el thread durante 500 ms (~0.5 segundos)
        usleep(16000);
    }

    return NULL;
}



char* generate_GameState_json(GameState *gameState) {

    // Create root object
    cJSON *gameStateJSON = cJSON_CreateObject();

    cJSON_AddStringToObject(gameStateJSON, "command", "sendGameState");

    // Add player information
    cJSON *playerJSON = cJSON_CreateObject();
    cJSON_AddNumberToObject(playerJSON, "positionX", gameState->player.position.x);
    cJSON_AddNumberToObject(playerJSON, "positionY", gameState->player.position.y);
    cJSON_AddNumberToObject(playerJSON, "sizeX", gameState->player.size.x);
    cJSON_AddNumberToObject(playerJSON, "sizeY", gameState->player.size.y);
    cJSON_AddNumberToObject(playerJSON, "lives", gameState->player.life);
    cJSON_AddNumberToObject(playerJSON, "score", gameState->player.score);
    cJSON_AddItemToObject(gameStateJSON, "player", playerJSON);

    // Add active balls
    cJSON *ballsArray = cJSON_CreateArray();
    for (int i = 0; i < gameState->maxBalls; i++) {
        cJSON *ballJSON = cJSON_CreateObject();
        cJSON_AddBoolToObject(ballJSON, "active", gameState->balls[i].active);
        cJSON_AddNumberToObject(ballJSON, "positionX", gameState->balls[i].position.x);
        cJSON_AddNumberToObject(ballJSON, "positionY", gameState->balls[i].position.y);
        cJSON_AddItemToArray(ballsArray, ballJSON);
    }
    cJSON_AddItemToObject(gameStateJSON, "balls", ballsArray);

    // Add active bricks
    cJSON *bricksArray = cJSON_CreateArray();
    for (int i = 0; i < gameState->linesOfBricks; i++) {
        for (int j = 0; j < gameState->bricksPerLine; j++) {
            cJSON *brickJSON = cJSON_CreateObject();
            cJSON_AddBoolToObject(brickJSON, "active", gameState->bricks[i][j].active);
            cJSON_AddItemToArray(bricksArray, brickJSON);
        }
    }
    cJSON_AddItemToObject(gameStateJSON, "bricks", bricksArray);

    // Add game state (gameOver or not)
    cJSON_AddBoolToObject(gameStateJSON, "gameOver", gameState->gameOver);

    // Add pause state (paused or not)
    cJSON_AddBoolToObject(gameStateJSON, "paused", gameState->pause);

    cJSON_AddBoolToObject(gameStateJSON, "winner", gameState->winner);

    // Add completed levels
    cJSON_AddNumberToObject(gameStateJSON, "levelsCompleted", gameState->levelsCompleted);

    // Convertir el objeto JSON a una cadena de texto
    char *jsonString = cJSON_PrintUnformatted(gameStateJSON);

    // Clean up cJSON memory
    cJSON_Delete(gameStateJSON);

    return jsonString;  // Remember to free the memory after use
}

void sendGameState(GameState *gameState) {

    const char *json = generate_GameState_json(gameState);
    if (json != NULL) {
        //printf("GAME STATUS ENVIADO\n");
        ComServer_sendStatus(json);
    }
}


PowerType get_power_type(const char* power_str) {
    if (strcmp(power_str, "ADD_LIFE") == 0) return ADD_LIFE;
    if (strcmp(power_str, "ADD_BALL") == 0) return ADD_BALL;
    if (strcmp(power_str, "DOUBLE_RACKET") == 0) return DOUBLE_RACKET;
    if (strcmp(power_str, "HALF_RACKET") == 0) return HALF_RACKET;
    if (strcmp(power_str, "SPEED_UP") == 0) return SPEED_UP;
    if (strcmp(power_str, "SPEED_DOWN") == 0) return SPEED_DOWN;
    if (strcmp(power_str, "UPDATE_POINTS") == 0) return UPDATE_POINTS;
    return NONE;
}

void process_brick_update(const char* json_command) {

    GameState *gameState = getGameState();
    // Parsear el JSON
    cJSON* root = cJSON_Parse(json_command);
    if (root == NULL) {
        printf("Error parsing JSON\n");
        return;
    }

    // Extraer valores del JSON
    cJSON* row_item = cJSON_GetObjectItem(root, "row");
    cJSON* column_item = cJSON_GetObjectItem(root, "column");
    cJSON* power_item = cJSON_GetObjectItem(root, "power");

    if (!cJSON_IsNumber(row_item) || !cJSON_IsNumber(column_item) || !cJSON_IsString(power_item)) {
        printf("Invalid JSON format\n");
        cJSON_Delete(root);
        return;
    }

    int row = row_item->valueint-1;
    int column = column_item->valueint-1;
    const char* power_str = power_item->valuestring;



    // Obtener el tipo de poder
    PowerType power = get_power_type(power_str);

    // Validar límites de row y column
    if ((row < 0 || row >= gameState->linesOfBricks || column < 0 || column >= gameState->bricksPerLine) && power!=UPDATE_POINTS) {
        printf("Row (%d) or Column (%d) out of bounds. Max Row: %d, Max Columns: %d\n", row, column, gameState->linesOfBricks - 1, gameState->linesOfBricks - 1);
        cJSON_Delete(root);
        return;
    }
    if ((row < 0 || row >= gameState->linesOfBricks) && power==UPDATE_POINTS) {
        cJSON_Delete(root);
        return;
    }
    // Llamar a la función correspondiente
    switch (power) {
        case ADD_LIFE:
            update_brick_life(row, column);
            break;
        case ADD_BALL:
            update_brick_ball(row, column);
            break;
        case DOUBLE_RACKET:
            update_brick_doubleRacket(row, column);
            break;
        case HALF_RACKET:
            update_brick_halfRacket(row, column);
            break;
        case SPEED_UP:
            update_brick_speedUp(row, column);
            break;
        case SPEED_DOWN:
            update_brick_speedDown(row, column);
            break;
        case UPDATE_POINTS:
            update_brick_score(row,column);
            break;
        default:
            printf("No action for power: %s\n", power_str);
            break;
    }

    // Liberar memoria
    cJSON_Delete(root);
}
