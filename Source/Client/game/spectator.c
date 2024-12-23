/*
================================== LICENCIA ==================================================
MIT License
Copyright (c) 2024 José Bernardo Barquero Bonilla,
                   Jose Eduardo Campos Salazar,
                   Jimmy Feng Feng,
                   Alexander Montero Vargas
Consulta el archivo LICENSE para más detalles.
==============================================================================================
*/

// BIBLIOTECAS DE PROYECTO
#include "../game_status.h"
#include "spectator.h"


// BIBLIOTECAS EXTERNAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cjson/cJSON.h>



int selectedPlayerIndex = 0; // Índice del jugador seleccionado

// Declaración de la instancia única

static PlayerList *instance = NULL;


/* Function: InitializePlayerList
   Descripción:
     Inicializa una estructura `PlayerList`, configurando el número de jugadores en 0 y asignando cadenas vacías
     a los nombres y UUIDs de los jugadores.

   Params:
     playerList - Puntero a la estructura `PlayerList` que será inicializada.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `playerList` debe ser un puntero válido antes de llamar a esta función.

   Example:
     PlayerList playerList;
InitializePlayerList(&playerList);
// Inicializa la lista de jugadores con valores predeterminados.

Problems:
  - Problema: Si `playerList` es un puntero nulo, la función provocará un comportamiento indefinido.
    - Solución: Asegurarse de que `playerList` sea válido antes de la llamada.

References:
  - Ninguna referencia externa específica.
*/


void InitializePlayerList(PlayerList *playerList) {

    playerList->numPlayers = 0; // Inicializa el número de jugadores a 0


    // Inicializa los nombres y UUIDs a cadenas vacías

    for (int i = 0; i < 10; i++) {

        strcpy(playerList->playerNames[i], ""); // Asigna cadena vacía

        strcpy(playerList->playerUUIDs[i], ""); // Asigna cadena vacía

    }

}


/* Function: GetPlayerListInstance
   Descripción:
     Obtiene la instancia única de la estructura `PlayerList`. Si no existe, crea e inicializa una nueva instancia.

   Params:
     (Ninguno)

   Returns:
     - PlayerList*: Un puntero a la instancia única de `PlayerList`.

   Restriction:
     - La instancia devuelta debe ser manejada correctamente para evitar fugas de memoria.

   Example:
     PlayerList *playerList = GetPlayerListInstance();
     // Obtiene la instancia única de `PlayerList`.

   Problems:
     - Problema: Si no se puede asignar memoria para la instancia, se retorna un puntero nulo.
       - Solución: Manejar este caso con validaciones y mensajes de error.

   References:
     - Ninguna referencia externa específica.
*/

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

/* Function: processJson
   Descripción:
     Procesa una cadena JSON para llenar la estructura `PlayerList` con los datos obtenidos. La función
     busca específicamente el campo "command" con valor "ClientesLista" y extrae los campos "id" y "nombre"
     de cada jugador en el array "data".

   Params:
     jsonString - Cadena de texto que contiene el mensaje JSON recibido.
     playerList - Puntero a la estructura `PlayerList` que será llenada con los datos extraídos.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `jsonString` debe ser una cadena válida y no nula.
     - `playerList` debe ser un puntero válido antes de llamar a esta función.

   Example:
     const char *json = "{\"command\":\"ClientesLista\",\"data\":[{\"id\":\"123\",\"nombre\":\"Jugador1\"}]}";
     PlayerList playerList;
     InitializePlayerList(&playerList);
     processJson(json, &playerList);
     // Llena la estructura playerList con los datos obtenidos del JSON.

   Problems:
     - Problema: Si el JSON no tiene un formato válido, no se procesarán los datos.
       - Solución: Registrar un mensaje de error y manejar el caso.
     - Problema: Si el array "data" tiene más elementos que la capacidad de `PlayerList`, puede ocurrir un desbordamiento.
       - Solución: Limitar el número de jugadores procesados y registrar un mensaje cuando se alcance el límite.

   References:
     - cJSON Documentation: https://github.com/DaveGamble/cJSON
*/

void processJson(const char *jsonString, PlayerList *playerList) {

    cJSON *json = cJSON_Parse(jsonString);

    if (json == NULL) {

        printf("Error al parsear JSON EN PLAYERLIST????\n");

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

/* Function: espectadorGetList
   Descripción:
     Procesa un mensaje recibido que contiene la lista de jugadores y actualiza la instancia de `PlayerList` con los datos.

   Params:
     recibido - Cadena de texto que contiene el mensaje JSON con la lista de jugadores.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `recibido` debe ser una cadena válida y no nula.

   Example:
     const char *json = "{\"command\":\"ClientesLista\",\"data\":[{\"id\":\"123\",\"nombre\":\"Jugador1\"}]}";
     espectadorGetList(json);
     // Actualiza la lista de jugadores basada en el JSON recibido.

   Problems:
     - Problema: Si `recibido` no es un JSON válido, no se procesará correctamente.
       - Solución: Registrar un mensaje de error en caso de fallo en `processJson`.

   References:
     - Ninguna referencia externa específica.
*/

void espectadorGetList(const char *recibido) {
    PlayerList *playerList = GetPlayerListInstance();
    processJson(recibido,playerList);

}


/* Function: updateGameStateFromJson
   Descripción:
     Actualiza el estado del juego (`GameState`) basado en un mensaje JSON recibido. Procesa datos
     relacionados con el jugador, las bolas, los ladrillos y otros atributos del juego.

   Params:
     jsonString - Cadena de texto que contiene el mensaje JSON con el estado del juego.
     gameState - Puntero al estado del juego (`GameState *`) que será actualizado.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `jsonString` debe ser una cadena válida y no nula.
     - `gameState` debe ser un puntero válido antes de llamar a esta función.

   Example:
     const char *json = "{\"command\":\"sendGameState\", \"player\": {...}, \"balls\": [...], \"bricks\": [...]}";
     updateGameStateFromJson(json, gameState);
     // Actualiza el estado del juego basado en el JSON recibido.

   Problems:
     - Problema: Si `jsonString` no es un JSON válido, no se procesará correctamente.
       - Solución: Registrar un mensaje de error en caso de fallo en `cJSON_Parse`.
     - Problema: Si algún campo esperado no está presente en el JSON, los datos pueden quedar incompletos.
       - Solución: Validar que los campos estén presentes antes de usarlos.

   References:
     - cJSON Documentation: https://github.com/DaveGamble/cJSON
     */
void updateGameStateFromJson(const char *jsonString, GameState *gameState) {

    cJSON *json = cJSON_Parse(jsonString);

    if (json == NULL) {

        printf("Error al parsear JSON en UPDATEFROMJSON spectator:158\n");

        return;

    }


    cJSON *command = cJSON_GetObjectItem(json, "command");

    if (command != NULL && strcmp(command->valuestring, "sendGameState") == 0) {

        // Actualizar el jugador

        cJSON *playerJson = cJSON_GetObjectItem(json, "player");

        if (playerJson != NULL) {

            gameState->player.position.x = cJSON_GetObjectItem(playerJson, "positionX")->valueint;

            gameState->player.position.y = cJSON_GetObjectItem(playerJson, "positionY")->valueint;

            gameState->player.size.x = cJSON_GetObjectItem(playerJson, "sizeX")->valueint;

            gameState->player.size.y = cJSON_GetObjectItem(playerJson, "sizeY")->valueint;

            gameState->player.life = cJSON_GetObjectItem(playerJson, "lives")->valueint;

            gameState->player.score = cJSON_GetObjectItem(playerJson, "score")->valueint;

        }


        // Actualizar las bolas

        cJSON *ballsJson = cJSON_GetObjectItem(json, "balls");

        if (ballsJson != NULL && cJSON_IsArray(ballsJson)) {

            for (int i = 0; i < cJSON_GetArraySize(ballsJson) && i < gameState->maxBalls; i++) {

                cJSON *ballJson = cJSON_GetArrayItem(ballsJson, i);

                gameState->balls[i].active = cJSON_GetObjectItem(ballJson, "active")->valueint;

                gameState->balls[i].position.x = cJSON_GetObjectItem(ballJson, "positionX")->valueint;

                gameState->balls[i].position.y = cJSON_GetObjectItem(ballJson, "positionY")->valueint;

                // Suponiendo que la velocidad y el radio de la bola son constantes o se manejan de otra manera

            }

        }


        // Actualizar los ladrillos

        cJSON *bricksJson = cJSON_GetObjectItem(json, "bricks");

        if (bricksJson != NULL && cJSON_IsArray(bricksJson)) {

            for (int i = 0; i < cJSON_GetArraySize(bricksJson); i++) {

                cJSON *brickJson = cJSON_GetArrayItem(bricksJson, i);

                int row = i / gameState->bricksPerLine;
                int col = i % gameState->bricksPerLine;

                gameState->bricks[row][col].active = cJSON_GetObjectItem(brickJson, "active")->valueint;

                // Puedes agregar más propiedades del ladrillo si es necesario

            }

        }


        // Actualizar otros campos del estado del juego

        gameState->gameOver = cJSON_GetObjectItem(json, "gameOver")->valueint;

        gameState->pause = cJSON_GetObjectItem(json, "paused")->valueint;

        gameState->winner = cJSON_GetObjectItem(json, "winner")->valueint;

        gameState->levelsCompleted = cJSON_GetObjectItem(json, "levelsCompleted")->valueint;

    }


    cJSON_Delete(json); // Liberar la memoria del JSON

}

/* Function: espectadorUpdateGame
   Descripción:
     Actualiza el estado del juego (`GameState`) utilizando los datos recibidos en formato JSON.
     Asegura la exclusividad de acceso al `GameState` utilizando un mutex.

   Params:
     recibido - Cadena de texto que contiene el mensaje JSON con el estado del juego.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `recibido` debe ser una cadena válida y no nula.

   Example:
     espectadorUpdateGame("{\"command\":\"sendGameState\", ...}");
     // Actualiza el estado del juego basado en el mensaje recibido.

   Problems:
     - Problema: Si `recibido` es NULL o no se puede obtener el estado del juego, no se realizará ninguna operación.
       - Solución: Validar los parámetros antes de proceder y registrar errores.

   References:
     - Ninguna referencia externa específica.
*/

void espectadorUpdateGame(const char *recibido) {
    if (recibido == NULL) {
        fprintf(stderr, "Error: Datos recibidos son NULL\n");
        return;
    }

    GameState *gameState = getGameState();
    if (gameState == NULL) {
        fprintf(stderr, "Error: No se pudo obtener gameState\n");
        return;
    }

    pthread_mutex_lock(&gameStateMutex); // Bloquear el mutex antes de modificar gameState
    updateGameStateFromJson(recibido, gameState);
    pthread_mutex_unlock(&gameStateMutex); // Desbloquear el mutex después de la modificación
}

/* Function: DrawPlayerList
   Descripción:
     Dibuja en pantalla la lista de jugadores disponibles, permitiendo resaltar al jugador seleccionado.

   Params:
     (Ninguno)

   Returns:
     - void: No retorna valores.

   Restriction:
     - La lista de jugadores debe haber sido inicializada previamente.

   Example:
     DrawPlayerList();
     // Dibuja la lista de jugadores en pantalla.

   Problems:
     - Problema: Si la lista de jugadores no está inicializada, no se dibujará nada.
       - Solución: Validar la instancia antes de proceder y registrar un error.

   References:
     - Ninguna referencia externa específica.
*/

void DrawPlayerList() {

    PlayerList *playerLista = GetPlayerListInstance();

    if (playerLista == NULL) {
        fprintf(stderr, "Error: No se pudo obtener la lista de jugadores\n");
        return;
    }

    // Dibujar la lista de jugadores
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Título de la pantalla
    DrawText("Player List:", 100, 50, 30, DARKGRAY);

    // Dibujar cada jugador
    for (int i = 0; i < playerLista->numPlayers; i++) {
        Color textColor = (i == selectedPlayerIndex) ? BLUE : BLACK; // Color para el jugador seleccionado
        DrawText(playerLista->playerNames[i], 100, 100 + (i * 30), 20, textColor);
    }

    // Mensaje adicional si no hay jugadores
    if (playerLista->numPlayers == 0) {
        DrawText("No players yet!", 100, 100, 20, DARKGRAY);
    }

    EndDrawing();
}

/* Function: UpdatePlayerList
   Descripción:
     Permite al usuario navegar por la lista de jugadores utilizando las teclas de dirección y seleccionar
     un jugador al presionar Enter. Envía la selección al servidor.

   Params:
     players - Puntero a la estructura `PlayerList` que contiene la lista de jugadores.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `players` debe ser un puntero válido antes de llamar a esta función.

   Example:
     UpdatePlayerList(playerList);
     // Permite navegar y seleccionar jugadores de la lista.

   Problems:
     - Problema: Si no hay jugadores disponibles, la navegación y selección no tendrán efecto.
       - Solución: Validar que la lista no esté vacía y registrar mensajes para depuración.

   References:
     - Ninguna referencia externa específica.
*/

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
        ComServer *comServer = ComServer_create();
        comServer_sendChoosenPlayer(comServer,players->playerUUIDs[selectedPlayerIndex]);
        GameState *gameState =  getGameState();
        gameState->comunicationRunning=false;
        setCurrentScreen(SPECTATOR);
    }

}

/* Function: askForPlayerList
Descripción:
Envía periódicamente una solicitud al servidor para obtener la lista de jugadores disponibles mientras
la pantalla actual sea `OBSERVER_SELECT`.

Params:
arg - Puntero a la estructura `GameState`.

Returns:
- void*: Retorna NULL al finalizar.

Restriction:
- `arg` debe ser un puntero válido a la estructura `GameState`.

Example:
pthread_t thread;
pthread_create(&thread, NULL, askForPlayerList, gameState);
// Inicia un hilo para solicitar periódicamente la lista de jugadores.

Problems:
- Problema: Si `gameState` es NULL, el método no funcionará.
- Solución: Validar el parámetro antes de proceder.
- Problema: Si la conexión al servidor falla, las solicitudes no tendrán efecto.
- Solución: Manejar errores de conexión y registrar mensajes para depuración.

References:
- Ninguna referencia externa específica.
*/
void *askForPlayerList(void *arg) {
    GameState *gameState = (GameState *)arg;
    ComServer *comServer=ComServer_create();
    while (gameState->currentScreen==OBSERVER_SELECT) {
        ComServer_observerGetlist(comServer);
        sleep(5);
    }
    gameState->comunicationRunning=false;// Send the current game state
    return NULL;
}
