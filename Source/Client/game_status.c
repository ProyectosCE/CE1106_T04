
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

#include "game_status.h"
#include "configuracion/configuracion.h"

// BIBLIOTECAS EXTERNAS
#include <stdio.h>
#include <stdlib.h>


static GameState *gameStateInstance = NULL;
float brickSpacing = 5.0f;


/* Function: initializeLevelSpeedChanged
Descripción:
Inicializa el arreglo dinámico `levelSpeedChanged` dentro de la estructura `GameState`.
Este arreglo se utiliza para rastrear si la velocidad del juego ha cambiado en cada nivel.

Params:
gameStateInstance - Puntero a la instancia de `GameState` que será inicializada.

Returns:
- void: No retorna valores.

Restriction:
- `gameStateInstance` debe ser un puntero válido.
- La función supone que el número de niveles se calcula como `linesOfBricks / 2`.
- Se debe liberar la memoria asignada con `free` después de usar `levelSpeedChanged`.

Example:
initializeLevelSpeedChanged(gameState);
// Inicializa el arreglo levelSpeedChanged en la estructura GameState.

Problems:
- Problema: Si no se puede asignar memoria para `levelSpeedChanged`, el programa finalizará con un error.
- Solución: Manejar el error con `perror` y finalizar la ejecución.
- Problema: Si `linesOfBricks` no es un valor divisible entre 2, el cálculo del número de niveles podría ser incorrecto.
- Solución: Validar la lógica del cálculo de niveles en el contexto del juego.

References:
- Ninguna referencia externa específica.
*/
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

/* Function: initGameState
Descripción:
Inicializa la instancia única de `GameState`, asignando memoria para las estructuras necesarias
(bolas, ladrillos, y configuración del juego) y estableciendo valores predeterminados.

Params:
(Ninguno)

Returns:
- void: No retorna valores.

Restriction:
- La función debe llamarse una sola vez en la ejecución para inicializar `gameStateInstance`.
- La memoria asignada dinámicamente debe liberarse adecuadamente al finalizar el juego.

Example:
initGameState();
// Inicializa la instancia de GameState para el juego.

Problems:
- Problema: Si no se puede asignar memoria para `gameStateInstance` o sus estructuras internas, el programa finalizará.
- Solución: Manejar errores de asignación de memoria con `perror` y finalizar adecuadamente.
- Problema: Si los valores de configuración obtenidos son inválidos, podrían causar comportamientos no deseados.
- Solución: Validar las configuraciones antes de usarlas.

References:
- Ninguna referencia externa específica.
*/

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
/* Function: getCurrentScreen
   Descripción:
     Obtiene la pantalla actual del juego desde la instancia única de `GameState`. Si `gameStateInstance`
     no está inicializado, lo inicializa.

   Params:
     (Ninguno)

   Returns:
     - GameScreen: Pantalla actual del juego.

   Restriction:
     - Requiere que `initGameState` esté correctamente implementado.

   Example:
     GameScreen currentScreen = getCurrentScreen();
     // Retorna la pantalla actual del juego.

   Problems:
     - Problema: Si `initGameState` falla, no se podrá obtener la pantalla.
       - Solución: Manejar errores de inicialización en `initGameState`.

   References:
     - Ninguna referencia externa específica.
*/
GameScreen getCurrentScreen() {
    if (gameStateInstance == NULL) initGameState();
    return gameStateInstance->currentScreen;
}

/* Function: getGameState
   Descripción:
     Retorna la instancia única de `GameState`. Si no está inicializada, la inicializa.

   Params:
     (Ninguno)

   Returns:
     - GameState*: Puntero a la instancia única de `GameState`.

   Restriction:
     - Requiere que `initGameState` esté correctamente implementado.

   Example:
     GameState *gameState = getGameState();
     // Obtiene la instancia de GameState.

   Problems:
     - Problema: Si `initGameState` falla, no se podrá obtener el estado del juego.
       - Solución: Manejar errores de inicialización en `initGameState`.

   References:
     - Ninguna referencia externa específica.
*/
GameState *getGameState() {
    if (gameStateInstance == NULL) initGameState();
    return gameStateInstance;
}

/* Function: isCameraEnabled
   Descripción:
     Verifica si la cámara está habilitada en el estado actual del juego. Inicializa `gameStateInstance`
     si no está definido.

   Params:
     (Ninguno)

   Returns:
     - bool: `true` si la cámara está habilitada, `false` en caso contrario.

   Restriction:
     - Requiere que `initGameState` esté correctamente implementado.

   Example:
     if (isCameraEnabled()) {
         printf("Cámara habilitada.\n");
     }

   Problems:
     - Problema: Si `initGameState` falla, no se puede obtener el estado de la cámara.
       - Solución: Manejar errores en `initGameState`.

   References:
     - Ninguna referencia externa específica.
*/
bool isCameraEnabled() {
    if (gameStateInstance == NULL) initGameState();
    return gameStateInstance->isCameraEnabled;
}
bool isControllerActive() {
  if (gameStateInstance == NULL) initGameState();
  return gameStateInstance->isControllerActive;
}

/* Function: isPlayer
   Descripción:
     Verifica si el juego está en modo jugador. Inicializa `gameStateInstance` si no está definido.

   Params:
     (Ninguno)

   Returns:
     - bool: `true` si el juego está en modo jugador, `false` en modo espectador.

   Restriction:
     - Requiere que `initGameState` esté correctamente implementado.

   Example:
     if (isPlayer()) {
         printf("Modo jugador activo.\n");
     }

   Problems:
     - Problema: Si `initGameState` falla, no se puede obtener el estado del modo.
       - Solución: Manejar errores en `initGameState`.

   References:
     - Ninguna referencia externa específica.
*/

bool isPlayer() {
    if (gameStateInstance == NULL) initGameState();
    return gameStateInstance->isPlayer;
}

// Setters

/* Function: setCurrentScreen
   Descripción:
     Establece la pantalla actual del juego en `gameStateInstance`. Inicializa `gameStateInstance` si no está definido.

   Params:
     screen - Nueva pantalla de tipo `GameScreen`.

   Returns:
     - void: No retorna valores.

   Restriction:
     - Requiere que `initGameState` esté correctamente implementado.

   Example:
     setCurrentScreen(GAME);
     // Cambia la pantalla actual a la de juego.

   Problems:
     - Problema: Si `initGameState` falla, no se podrá establecer la pantalla.
       - Solución: Manejar errores en `initGameState`.

   References:
     - Ninguna referencia externa específica.
*/

void setCurrentScreen(GameScreen screen) {
    if (gameStateInstance == NULL) initGameState();
    gameStateInstance->currentScreen = screen;
}

/* Function: setCameraEnabled
   Descripción:
     Habilita o deshabilita la cámara en el estado del juego. Inicializa `gameStateInstance` si no está definido.

   Params:
     enabled - Valor booleano que indica si la cámara debe estar habilitada.

   Returns:
     - void: No retorna valores.

   Restriction:
     - Requiere que `initGameState` esté correctamente implementado.

   Example:
     setCameraEnabled(true);
     // Habilita la cámara.

   Problems:
     - Problema: Si `initGameState` falla, no se puede modificar el estado de la cámara.
       - Solución: Manejar errores en `initGameState`.

   References:
     - Ninguna referencia externa específica.
*/
void setCameraEnabled(bool enabled) {
    if (gameStateInstance == NULL) initGameState();
    gameStateInstance->isCameraEnabled = enabled;
}

/* Function: setPlayer
Descripción:
Cambia el modo del juego entre jugador y espectador. Inicializa `gameStateInstance` si no está definido.

Params:
player - Valor booleano que indica si el modo debe ser jugador (`true`) o espectador (`false`).

Returns:
- void: No retorna valores.

Restriction:
- Requiere que `initGameState` esté correctamente implementado.

Example:
setPlayer(true);
// Cambia el juego a modo jugador.

Problems:
- Problema: Si `initGameState` falla, no se puede cambiar el modo.
- Solución: Manejar errores en `initGameState`.

*/
void setPlayer(bool player) {
    if (gameStateInstance == NULL) initGameState();
    gameStateInstance->isPlayer = player;
}


void setControllerEnable() {
  if (!gameStateInstance) initGameState();
  gameStateInstance->isControllerActive = !gameStateInstance->isControllerActive;
}