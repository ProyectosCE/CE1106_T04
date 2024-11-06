#include "game_logic.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cjson/cJSON.h>

#include "raylib.h"
#include "Objects/player.h"
#include "Objects/ball.h"

#include "../comunicaciones/comServer.h"

#include "collision_handler.h"

void update_game(GameState *gameState) {
    if (IsKeyPressed(KEY_P)) gameState->pause = !gameState->pause;

    if (!gameState->pause) {

        update_player_movement(&gameState->player, screenWidth);
        update_ball_positions(&gameState->player, gameState->balls);
        handle_collisions(gameState);

        // Verificar si el jugador pierde todas las vidas
        if (gameState->player.life <= 0) {
            gameState->gameOver = true;  // Cambiar al menú principal en caso de Game Over
        }
    }
    if (gameState->gameOver) {
        if (IsKeyPressed(KEY_ENTER)) gameState->restart=true;
    }

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
    for (int i = 0; i < MAX_BALLS; i++) {
        cJSON *ballJSON = cJSON_CreateObject();
        cJSON_AddBoolToObject(ballJSON, "active", gameState->balls[i].active);
        cJSON_AddNumberToObject(ballJSON, "positionX", gameState->balls[i].position.x);
        cJSON_AddNumberToObject(ballJSON, "positionY", gameState->balls[i].position.y);
        cJSON_AddItemToArray(ballsArray, ballJSON);
    }
    cJSON_AddItemToObject(gameStateJSON, "balls", ballsArray);

    // Add active bricks
    cJSON *bricksArray = cJSON_CreateArray();
    for (int i = 0; i < LINES_OF_BRICKS; i++) {
        for (int j = 0; j < BRICKS_PER_LINE; j++) {
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
    if (json!=NULL) {
        printf("GAME STATUS ENVIADO\n");
        ComServer_sendStatus(json);
    }

}