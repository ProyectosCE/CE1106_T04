//
// Created by jimmy on 12/10/24.
//

#include "spectator.h"



/*
// Función para parsear el JSON y llenar GameState
void parseGameState(GameState *gameState, const char *json) {
    cJSON *root = cJSON_Parse(json);
    if (!root) return;

    cJSON *brickSize = cJSON_GetObjectItem(root, "brickSize");
    gameState->brickSizeX = cJSON_GetObjectItem(brickSize, "sizeX")->valuedouble;
    gameState->brickSizeY = cJSON_GetObjectItem(brickSize, "sizeY")->valuedouble;


    // Parsear información del jugador
    cJSON *player = cJSON_GetObjectItem(root, "player");
    gameState->player.positionX = cJSON_GetObjectItem(player, "positionX")->valuedouble;
    gameState->player.positionY = cJSON_GetObjectItem(player, "positionY")->valuedouble;
    gameState->player.sizeX = cJSON_GetObjectItem(player, "sizeX")->valuedouble;
    gameState->player.sizeY = cJSON_GetObjectItem(player, "sizeY")->valuedouble;
    gameState->player.life = cJSON_GetObjectItem(player, "life")->valueint;
    gameState->player.score = cJSON_GetObjectItem(player, "score")->valueint;

    // Parsear el array de bolas
    cJSON *ballsArray = cJSON_GetObjectItem(root, "balls");
    int ballCount = cJSON_GetArraySize(ballsArray);
    gameState->balls = (BallSpectator *)malloc(ballCount * sizeof(BallSpectator));
    for (int i = 0; i < ballCount; i++) {
        cJSON *ball = cJSON_GetArrayItem(ballsArray, i);
        gameState->balls[i].positionX = cJSON_GetObjectItem(ball, "positionX")->valuedouble;
        gameState->balls[i].positionY = cJSON_GetObjectItem(ball, "positionY")->valuedouble;
    }

    // Parsear el array de ladrillos
    cJSON *bricksArray = cJSON_GetObjectItem(root, "bricks");
    int brickCount = cJSON_GetArraySize(bricksArray);
    gameState->bricks = (BrickSpectator *)malloc(brickCount * sizeof(BrickSpectator));
    for (int i = 0; i < brickCount; i++) {
        cJSON *brick = cJSON_GetArrayItem(bricksArray, i);
        gameState->bricks[i].positionX = cJSON_GetObjectItem(brick, "positionX")->valuedouble;
        gameState->bricks[i].positionY = cJSON_GetObjectItem(brick, "positionY")->valuedouble;

        // Asignar color basado en posición o nivel, ajusta esto según lo necesario
        if (gameState->bricks[i].positionY <= 70) gameState->bricks[i].color = RED;
        else if (gameState->bricks[i].positionY <= 110) gameState->bricks[i].color = ORANGE;
        else if (gameState->bricks[i].positionY <= 150) gameState->bricks[i].color = YELLOW;
        else gameState->bricks[i].color = GREEN;
    }

    // Parsear los otros elementos del juego
    gameState->lives = cJSON_GetObjectItem(root, "lives")->valueint;
    gameState->score = cJSON_GetObjectItem(root, "score")->valueint;
    gameState->levelsCompleted = cJSON_GetObjectItem(root, "levelsCompleted")->valueint;
    gameState->gameOver = cJSON_GetObjectItem(root, "gameOver")->valueint;

    cJSON_Delete(root);
}


// Función para leer el archivo JSON
char* readJSONFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char *)malloc(length + 1);
    if (content) {
        fread(content, 1, length, file);
        content[length] = '\0';
    }
    fclose(file);
    return content;
}

*/