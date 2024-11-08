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

// BIBLIOTECAS EXTERNAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cjson/cJSON.h>
#include "raylib.h"

// BIBLIOTECAS DE PROYECTO
#include "game_logic.h"
#include "Objects/player.h"
#include "Objects/ball.h"
#include "../comunicaciones/comServer.h"
#include "collision_handler.h"
#include "powerHandler.h"


/* Function: update_game
   Descripción:
     Actualiza el estado del juego durante cada iteración del ciclo principal. Gestiona la pausa, el movimiento
     del jugador y las pelotas, maneja las colisiones, y verifica las condiciones de fin de juego.

   Params:
     gameState - Puntero al estado global del juego que contiene toda la información relevante para
                 actualizar la lógica del juego, como el jugador, las pelotas y los bloques.

   Returns:
     - void: Esta función no devuelve valores.

   Restriction:
     - Se debe garantizar que el `gameState` esté correctamente inicializado antes de llamar a esta función.
     - Las funciones auxiliares como `update_player_movement`, `update_ball_positions`, y `handle_collisions`
       deben estar correctamente implementadas para un funcionamiento esperado.

   Example:
     update_game(&gameState);
     // Llama a esta función dentro del bucle principal del juego para mantener actualizada la lógica del juego.

   Problems:
     - Problema: Si los métodos auxiliares generan errores, la lógica principal del juego podría fallar.
       - Solución: Asegurar un manejo adecuado de errores y pruebas exhaustivas de las funciones dependientes.

   References:
     - Ninguna referencia externa específica.
*/

void update_game(GameState *gameState) {
    if (IsKeyPressed(KEY_P)) gameState->pause = !gameState->pause; // Alterna la pausa al presionar 'P'.

    if (!gameState->pause) {
        // Actualiza el movimiento del jugador.
        update_player_movement(&gameState->player, screenWidth);

        // Actualiza la posición de las pelotas.
        update_ball_positions(&gameState->player, gameState->balls);

        // Maneja las colisiones entre objetos del juego.
        handle_collisions(gameState);

        // Verifica si todas las pelotas están inactivas y si ya se lanzaron.
        if (noBallsActive(gameState->balls, gameState->maxBalls) && gameState->bolaLanzada) {
            gameState->player.life--;      // Resta una vida al jugador.
            gameState->bolaLanzada = false; // Resetea el estado de lanzamiento.
        }

        // Verifica si el jugador ha perdido todas sus vidas.
        if (gameState->player.life <= 0) {
            gameState->gameOver = true;    // Marca el estado de Game Over.
        }

        // Verifica si se han eliminado todos los bloques.
        if (bloquesEliminados(gameState)) {
            gameState->winner = true;     // Marca al jugador como ganador.
        }
    }

    // Maneja la transición a reinicio del juego en caso de Game Over o victoria.
    if (gameState->gameOver && !gameState->winner) {
        if (IsKeyPressed(KEY_ENTER)) gameState->restart = true;
    }
    // Maneja la transición a reinicio del juego en caso de Game Over o victoria.
    if (gameState->winner) {
        if (IsKeyPressed(KEY_ENTER)) gameState->restart = true;
    }
}


/* Function: send_game_state_thread
   Descripción:
     Hilo encargado de enviar el estado del juego a los clientes de forma periódica. Crea una copia local
     del estado del juego protegido por un mutex para garantizar consistencia y evitar condiciones de carrera.

   Params:
     arg - Puntero al estado global del juego (`GameState *`) que contiene toda la información necesaria
           para enviar el estado del juego a los clientes.

   Returns:
     - void*: Devuelve `NULL` al finalizar la ejecución del hilo.

   Restriction:
     - Se debe garantizar que el `GameState` esté correctamente inicializado antes de pasar como argumento.
     - El mutex `gameStateMutex` debe estar correctamente configurado para proteger el acceso al estado.

   Example:
     pthread_t sendStateThread;
     pthread_create(&sendStateThread, NULL, send_game_state_thread, (void *)&gameState);
     // Crea un hilo para enviar el estado del juego periódicamente a los clientes.

   Problems:
     - Problema: Si el `GameState` es `NULL`, el hilo no puede proceder.
       - Solución: Manejo temprano del error con un mensaje en `stderr` y terminación del hilo.
     - Problema: Si no se usa correctamente el mutex, pueden ocurrir condiciones de carrera.
       - Solución: Bloquear y desbloquear correctamente el mutex con `pthread_mutex_lock` y `pthread_mutex_unlock`.

   References:
     - pthread_mutex_lock: https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html
     - usleep: https://man7.org/linux/man-pages/man3/usleep.3.html
*/

void *send_game_state_thread(void *arg) {
    GameState *gameState = (GameState *)arg; // Castea el argumento al tipo `GameState`.

    if (gameState == NULL) {
        // Manejo de error si el estado del juego no está inicializado.
        fprintf(stderr, "Error: gameState es NULL\n");
        return NULL;
    }

    while (true) {
        // Crear una copia local del estado del juego.
        GameState localState;

        // Bloquea el mutex para proteger el acceso al estado global del juego.
        pthread_mutex_lock(&gameStateMutex);
        bool isRunning = gameState->running; // Verifica si el juego sigue en ejecución.
        if (isRunning) {
            localState = *gameState; // Copia el estado del juego al local.
        }
        pthread_mutex_unlock(&gameStateMutex); // Desbloquea el mutex.

        if (!isRunning) {
            break; // Termina el hilo si el juego no está en ejecución.
        }

        // Enviar el estado del juego usando la copia local.
        sendGameState(&localState);

        // Pausa el hilo durante 20 ms (~0.02 segundos).
        usleep(20000);
    }

    return NULL; // Finaliza el hilo devolviendo `NULL`.
}



/* Function: generate_GameState_json
   Descripción:
     Genera una representación JSON del estado actual del juego a partir del objeto `GameState`.
     Incluye información sobre el jugador, las pelotas activas, los ladrillos, y el estado general del juego.

   Params:
     gameState - Puntero al estado global del juego (`GameState *`), que contiene todos los detalles necesarios
                 para construir la representación JSON.

   Returns:
     - char*: Cadena de texto con la representación JSON del estado del juego.
       Nota: La memoria de la cadena debe liberarse después de usarla para evitar fugas de memoria.

   Restriction:
     - Se debe garantizar que el `gameState` esté correctamente inicializado antes de llamar a esta función.
     - El llamador debe liberar la memoria de la cadena JSON devuelta con `free`.

   Example:
     GameState *gameState = getGameState();
     char *jsonString = generate_GameState_json(gameState);
     printf("JSON: %s\n", jsonString);
     free(jsonString); // Liberar la memoria asignada.

   Problems:
     - Problema: Si algún campo del `GameState` no está inicializado, los datos generados en el JSON pueden
       ser inconsistentes o incorrectos.
       - Solución: Garantizar que los campos del `GameState` estén completamente inicializados antes de llamar
         a esta función.

   References:
     - cJSON Documentation: https://github.com/DaveGamble/cJSON
*/

char* generate_GameState_json(GameState *gameState) {
    // Crear el objeto JSON raíz
    cJSON *gameStateJSON = cJSON_CreateObject();

    cJSON_AddStringToObject(gameStateJSON, "command", "sendGameState");

    // Agregar información del jugador
    cJSON *playerJSON = cJSON_CreateObject();
    cJSON_AddNumberToObject(playerJSON, "positionX", gameState->player.position.x);
    cJSON_AddNumberToObject(playerJSON, "positionY", gameState->player.position.y);
    cJSON_AddNumberToObject(playerJSON, "sizeX", gameState->player.size.x);
    cJSON_AddNumberToObject(playerJSON, "sizeY", gameState->player.size.y);
    cJSON_AddNumberToObject(playerJSON, "lives", gameState->player.life);
    cJSON_AddNumberToObject(playerJSON, "score", gameState->player.score);
    cJSON_AddItemToObject(gameStateJSON, "player", playerJSON);

    // Agregar información de las pelotas activas
    cJSON *ballsArray = cJSON_CreateArray();
    for (int i = 0; i < gameState->maxBalls; i++) {
        cJSON *ballJSON = cJSON_CreateObject();
        cJSON_AddBoolToObject(ballJSON, "active", gameState->balls[i].active);
        cJSON_AddNumberToObject(ballJSON, "positionX", gameState->balls[i].position.x);
        cJSON_AddNumberToObject(ballJSON, "positionY", gameState->balls[i].position.y);
        cJSON_AddItemToArray(ballsArray, ballJSON);
    }
    cJSON_AddItemToObject(gameStateJSON, "balls", ballsArray);

    // Agregar información de los ladrillos activos
    cJSON *bricksArray = cJSON_CreateArray();
    for (int i = 0; i < gameState->linesOfBricks; i++) {
        for (int j = 0; j < gameState->bricksPerLine; j++) {
            cJSON *brickJSON = cJSON_CreateObject();
            cJSON_AddBoolToObject(brickJSON, "active", gameState->bricks[i][j].active);
            cJSON_AddItemToArray(bricksArray, brickJSON);
        }
    }
    cJSON_AddItemToObject(gameStateJSON, "bricks", bricksArray);

    // Agregar estado general del juego
    cJSON_AddBoolToObject(gameStateJSON, "gameOver", gameState->gameOver);
    cJSON_AddBoolToObject(gameStateJSON, "paused", gameState->pause);
    cJSON_AddBoolToObject(gameStateJSON, "winner", gameState->winner);
    cJSON_AddNumberToObject(gameStateJSON, "levelsCompleted", gameState->levelsCompleted);

    // Convertir el objeto JSON a una cadena
    char *jsonString = cJSON_PrintUnformatted(gameStateJSON);

    // Liberar memoria utilizada por cJSON
    cJSON_Delete(gameStateJSON);

    return jsonString;  // Recordar liberar esta memoria con `free`
}


/* Function: sendGameState
   Descripción:
     Envía el estado actual del juego a través del servidor de comunicación en formato JSON.

   Params:
     gameState - Puntero al estado global del juego (`GameState *`) que contiene la información
                 necesaria para generar y enviar el estado del juego.

   Returns:
     - void: Esta función no devuelve valores.

   Restriction:
     - Se debe garantizar que `gameState` esté correctamente inicializado antes de llamar a esta función.
     - La función `generate_GameState_json` debe estar correctamente implementada.

   Example:
     GameState *gameState = getGameState();
     sendGameState(gameState);
     // Envía el estado actual del juego a los clientes conectados.

*/

void sendGameState(GameState *gameState) {
    const char *json = generate_GameState_json(gameState); // Generar el estado del juego en formato JSON.
    if (json != NULL) {
        ComServer_sendStatus(json); // Enviar el estado del juego al servidor de comunicación.
    }
}



/* Function: get_power_type
   Descripción:
     Convierte una cadena de texto que representa un tipo de poder en el enumerador correspondiente
     del tipo `PowerType`.

   Params:
     power_str - Cadena de texto que representa el tipo de poder. Debe coincidir exactamente
                 con los valores definidos (e.g., "ADD_LIFE", "ADD_BALL").

   Returns:
     - PowerType: Retorna el valor correspondiente del enumerador `PowerType`.
       Si no coincide con ninguno, retorna `NONE`.

   Restriction:
     - `power_str` debe ser una cadena válida. La comparación es sensible a mayúsculas/minúsculas.

   Example:
     PowerType power = get_power_type("ADD_LIFE");
     if (power == ADD_LIFE) {
         printf("El poder es de tipo 'ADD_LIFE'.\n");
     }

   Problems:
     - Problema: Si `power_str` no coincide con ninguno de los valores esperados, retorna `NONE`.
       - Solución: Asegurarse de que las cadenas sean consistentes y estén correctamente definidas.

   References:
     - strcmp: https://man7.org/linux/man-pages/man3/strcmp.3.html
*/

PowerType get_power_type(const char* power_str) {
    if (strcmp(power_str, "ADD_LIFE") == 0) return ADD_LIFE;
    if (strcmp(power_str, "ADD_BALL") == 0) return ADD_BALL;
    if (strcmp(power_str, "DOUBLE_RACKET") == 0) return DOUBLE_RACKET;
    if (strcmp(power_str, "HALF_RACKET") == 0) return HALF_RACKET;
    if (strcmp(power_str, "SPEED_UP") == 0) return SPEED_UP;
    if (strcmp(power_str, "SPEED_DOWN") == 0) return SPEED_DOWN;
    if (strcmp(power_str, "UPDATE_POINTS") == 0) return UPDATE_POINTS;
    return NONE; // Retorna NONE si no hay coincidencia.
}

/* Function: process_brick_update
   Descripción:
     Procesa una actualización de un ladrillo basado en un comando recibido en formato JSON.
     Extrae la fila, la columna y el tipo de poder del JSON para aplicar la acción correspondiente.

   Params:
     json_command - Cadena en formato JSON que contiene los datos necesarios para actualizar un ladrillo.
                    El JSON debe incluir los siguientes campos:
                      - "row" (entero): Fila del ladrillo a actualizar.
                      - "column" (entero): Columna del ladrillo a actualizar.
                      - "power" (cadena): Tipo de poder a aplicar (e.g., "ADD_LIFE", "SPEED_UP").

   Returns:
     - void: Esta función no devuelve valores.

   Restriction:
     - `json_command` debe ser un JSON válido con los campos necesarios.
     - El estado del juego (`GameState`) debe estar correctamente inicializado y accesible.

   Example:
     const char *command = "{\"row\": 2, \"column\": 3, \"power\": \"ADD_LIFE\"}";
     process_brick_update(command);
     // Actualiza el ladrillo en la fila 2, columna 3, aplicando el poder "ADD_LIFE".

   Problems:
     - Problema: Si el JSON no es válido o le faltan campos requeridos, no se aplicará ninguna acción.
       - Solución: Validar el formato del JSON antes de llamarlo y manejar los errores de parsing.
     - Problema: Si los índices de fila o columna están fuera de los límites del tablero, no se aplica ninguna acción.
       - Solución: Garantizar que los valores de "row" y "column" estén dentro de los límites definidos.

   References:
     - cJSON Documentation: https://github.com/DaveGamble/cJSON
*/

void process_brick_update(const char* json_command) {
    GameState *gameState = getGameState(); // Obtiene el estado global del juego.

    // Parsear el comando JSON.
    cJSON* root = cJSON_Parse(json_command);
    if (root == NULL) {
        printf("Error parsing JSON\n");
        return;
    }

    // Extraer valores del JSON.
    cJSON* row_item = cJSON_GetObjectItem(root, "row");
    cJSON* column_item = cJSON_GetObjectItem(root, "column");
    cJSON* power_item = cJSON_GetObjectItem(root, "power");

    // Validar formato del JSON.
    if (!cJSON_IsNumber(row_item) || !cJSON_IsNumber(column_item) || !cJSON_IsString(power_item)) {
        printf("Invalid JSON format\n");
        cJSON_Delete(root);
        return;
    }

    int row = row_item->valueint - 1;  // Ajustar índice a base 0.
    int column = column_item->valueint - 1;
    const char* power_str = power_item->valuestring;

    // Obtener el tipo de poder.
    PowerType power = get_power_type(power_str);

    // Validar límites de fila y columna.
    if ((row < 0 || row >= gameState->linesOfBricks || column < 0 || column >= gameState->bricksPerLine) && power != UPDATE_POINTS) {
        printf("Row (%d) or Column (%d) out of bounds. Max Row: %d, Max Columns: %d\n", row, column, gameState->linesOfBricks - 1, gameState->linesOfBricks - 1);
        cJSON_Delete(root);
        return;
    }
    if ((row < 0 || row >= gameState->linesOfBricks) && power == UPDATE_POINTS) {
        cJSON_Delete(root);
        return;
    }

    // Aplicar la acción correspondiente según el tipo de poder.
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
            update_brick_score(row, column);
            break;
        default:
            printf("No action for power: %s\n", power_str);
            break;
    }

    // Liberar memoria asignada por cJSON.
    cJSON_Delete(root);
}

