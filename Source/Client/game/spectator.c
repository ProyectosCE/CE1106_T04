//
// Created by jimmy on 12/10/24.
//

#include "spectator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

#include "../game_status.h"

int selectedPlayerIndex = 0; // Índice del jugador seleccionado

// Declaración de la instancia única

static PlayerList *instance = NULL;


// Función para inicializar la estructura PlayerList

void InitializePlayerList(PlayerList *playerList) {

    playerList->numPlayers = 0; // Inicializa el número de jugadores a 0


    // Inicializa los nombres y UUIDs a cadenas vacías

    for (int i = 0; i < 10; i++) {

        strcpy(playerList->playerNames[i], ""); // Asigna cadena vacía

        strcpy(playerList->playerUUIDs[i], ""); // Asigna cadena vacía

    }

}


// Función para obtener la instancia única de PlayerList

PlayerList* GetPlayerListInstance() {

    if (instance == NULL) {

        // Si no existe, se crea una nueva instancia

        instance = (PlayerList *)malloc(sizeof(PlayerList));

        if (instance != NULL) {

            InitializePlayerList(instance);

        }

    }

    return instance;

}

// Función para procesar el JSON y llenar la estructura PlayerList

void processJson(const char *jsonString, PlayerList *playerList) {

    cJSON *json = cJSON_Parse(jsonString);

    if (json == NULL) {

        printf("Error al parsear JSON\n");

        return;

    }


    // Obtener el campo "command"

    cJSON *command = cJSON_GetObjectItem(json, "command");

    if (command != NULL && strcmp(command->valuestring, "ClientesLista") == 0) {

        // Obtener el campo "data"

        cJSON *data = cJSON_GetObjectItem(json, "data");

        if (data != NULL && cJSON_IsArray(data)) {

            playerList->numPlayers = 0; // Inicializar el número de jugadores


            // Iterar sobre cada elemento del array "data"

            cJSON *player = NULL;

            cJSON_ArrayForEach(player, data) {

                if (playerList->numPlayers >= 10) {

                    printf("Se alcanzó el límite máximo de jugadores.\n");

                    break; // Evitar desbordamiento

                }


                // Obtener el "id" y "nombre"

                cJSON *id = cJSON_GetObjectItem(player, "id");

                cJSON *nombre = cJSON_GetObjectItem(player, "nombre");


                if (id != NULL && nombre != NULL) {

                    // Asignar valores a la estructura

                    strncpy(playerList->playerUUIDs[playerList->numPlayers], id->valuestring, 36);

                    strncpy(playerList->playerNames[playerList->numPlayers], nombre->valuestring, 49);

                    playerList->playerNames[playerList->numPlayers][49] = '\0'; // Asegurarse de que esté terminada
                    playerList->playerUUIDs[playerList->numPlayers][36] = '\0'; // Asegurarse de que esté terminada

                    playerList->numPlayers++;

                }

            }

        }

    }


    // Liberar la memoria del objeto JSON

    cJSON_Delete(json);

}



void espectadorGetList(const char *recibido) {
    PlayerList *playerList = GetPlayerListInstance();
    processJson(recibido,playerList);

}

void DrawPlayerList(const PlayerList *players) {

    BeginDrawing();

    ClearBackground(RAYWHITE);


    // Título de la pantalla

    DrawText("Player List:", 100, 50, 30, DARKGRAY);


    // Dibujar la lista de jugadores

    for (int i = 0; i < players->numPlayers; i++) {

        Color textColor = (i == selectedPlayerIndex) ? BLUE : BLACK; // Cambiar color si es el jugador seleccionado

        DrawText(players->playerNames[i], 100, 100 + (i * 30), 20, textColor);

    }


    // Mensaje adicional (por ejemplo, si no hay jugadores)

    if (players->numPlayers == 0) {

        DrawText("No players yet!", 100, 100, 20, DARKGRAY);

    }


    // Finalizar el dibujo

    EndDrawing();

}


void UpdatePlayerList(const PlayerList *players) {

    // Navega entre los jugadores disponibles

    if (IsKeyPressed(KEY_UP)) {

        selectedPlayerIndex--;

        if (selectedPlayerIndex < 0) {

            selectedPlayerIndex = 0; // Limitar el índice al mínimo

        }

    }

    else if (IsKeyPressed(KEY_DOWN)) {

        selectedPlayerIndex++;

        if (selectedPlayerIndex >= players->numPlayers) {

            selectedPlayerIndex = players->numPlayers - 1; // Limitar el índice al máximo

        }

    }


    // Seleccionar el jugador al presionar Enter

    if (IsKeyPressed(KEY_ENTER)) {

        printf("Selected Player: %s (UUID: %s)\n", players->playerNames[selectedPlayerIndex], players->playerUUIDs[selectedPlayerIndex]);
        setCurrentScreen(SPECTATOR);
        ComServer *comServer = ComServer_create();
        comServer_sendChoosenPlayer(comServer,players->playerUUIDs[selectedPlayerIndex]);
    }

}


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