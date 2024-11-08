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
#include "powerHandler.h"
#include "../game_status.h"
#include "Objects/player.h"

GameState *gameStateHandler;

/* Function: initPowerHandler
   Descripción:
     Inicializa el manejador de poderes asignando el estado global del juego a `gameStateHandler`.

   Params:
     (Ninguno)

   Returns:
     - void: No retorna valores.

   Restriction:
     - La función `getGameState` debe estar correctamente implementada para devolver un puntero válido
       al estado global del juego.

   Example:
     initPowerHandler();
     // Inicializa el manejador de poderes con el estado actual del juego.

   Problems:
     - Problema: Si `getGameState` devuelve un valor nulo, el manejador de poderes no se inicializará correctamente.
       - Solución: Verificar el resultado de `getGameState` y manejar casos nulos.

   References:
     - Ninguna referencia externa específica.
*/

void initPowerHandler() {
    gameStateHandler = getGameState();
}

/* Function: update_brick_score
Descripción:
Actualiza los puntos de los ladrillos activos de un nivel específico. Incrementa los niveles y los puntos
en función de la lógica actual.

Params:
level - Nivel del juego en el que se actualizarán los ladrillos.
new_points - Nuevos puntos que se asignarán a los ladrillos activos en el nivel.

Returns:
- void: No retorna valores.

Restriction:
- `gameStateHandler` debe estar correctamente inicializado antes de llamar a esta función.
- La lógica de niveles y líneas debe ser consistente con los valores establecidos.

Example:
update_brick_score(2, 100);
// Actualiza los ladrillos activos en el nivel 2 con 100 puntos.

Problems:
- Problema: Si `gameStateHandler` no está inicializado, se producirán errores de referencia nula.
- Solución: Validar que `gameStateHandler` no sea nulo antes de usarlo.
- Problema: Si `lines_per_level` no coincide con la configuración real, los cálculos de nivel
podrían ser incorrectos.
- Solución: Sincronizar `lines_per_level` con los parámetros globales del juego.

References:
- Ninguna referencia externa específica.
*/

void update_brick_score(int level, int new_points) {
    level++;  // Incrementa el nivel según tu lógica actual
    new_points++;  // Incrementa los puntos según tu lógica actual

    int lines_per_level = 2; // Cada nivel tiene 2 líneas

    for (int i = 0; i < gameStateHandler->linesOfBricks; i++) {
        for (int j = 0; j < gameStateHandler->bricksPerLine; j++) {
            if (gameStateHandler->bricks[i][j].active) {
                // Calcula el nivel actual para la línea i
                int current_level = gameStateHandler->linesOfBricks / lines_per_level - i / lines_per_level;

                // Actualiza puntos si la línea pertenece al nivel especificado
                if (current_level == level) {
                    gameStateHandler->bricks[i][j].points = new_points;
                }
            }
        }
    }
}


/* Function: add_ball
   Descripción:
     Activa una nueva bola en el juego asignándole una posición inicial específica y una velocidad predefinida.

   Params:
     posX - Coordenada X inicial de la nueva bola.
     posY - Coordenada Y inicial de la nueva bola.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `gameStateHandler` debe estar correctamente inicializado.
     - Debe haber al menos una bola inactiva disponible en el estado del juego.

   Example:
     add_ball(100, 200);
     // Agrega una bola activa en la posición (100, 200).

   Problems:
     - Problema: Si todas las bolas están activas, no se podrá agregar una nueva bola.
       - Solución: Manejar este caso según la lógica del juego (e.g., registrar un mensaje o aplicar otro comportamiento).
*/

void add_ball(int posX, int posY) {
    for (int i = 0; i < gameStateHandler->maxBalls; i++) {
        if (!gameStateHandler->balls[i].active) {
            gameStateHandler->balls[i].active = true;
            gameStateHandler->balls[i].position = (Vector2){ posX, posY };  // Posición del ladrillo destruido
            gameStateHandler->balls[i].speed = (Vector2){0, 5 * gameStateHandler->ball_speed_multiplier};
            break;  // Salir del bucle una vez que activamos una nueva bola
        }
    }
}

/* Function: doubleRacket
   Descripción:
     Duplica el tamaño horizontal de la raqueta del jugador hasta un límite máximo predefinido.

   Params:
     (Ninguno)

   Returns:
     - void: No retorna valores.

   Restriction:
     - `gameStateHandler` debe estar correctamente inicializado.
     - El tamaño de la raqueta no debe superar 40% del ancho de la pantalla.

   Example:
     doubleRacket();
     // Duplica el tamaño de la raqueta del jugador si no ha alcanzado el límite.

   Problems:
     - Problema: Si el tamaño de la raqueta no se actualiza correctamente, podrían surgir inconsistencias visuales.
*/

void doubleRacket(){
    if (! (gameStateHandler->player.size.x >= 4*(screenWidth/10))) {
        gameStateHandler->player.size = (Vector2){ gameStateHandler->player.size.x*2, 10 };
    }
}

/* Function: halfRacket
   Descripción:
     Reduce el tamaño horizontal de la raqueta del jugador a la mitad hasta un límite mínimo predefinido.

   Params:
     (Ninguno)

   Returns:
     - void: No retorna valores.

   Restriction:
     - `gameStateHandler` debe estar correctamente inicializado.
     - El tamaño de la raqueta no debe ser menor al 10% del ancho de la pantalla.

   Example:
     halfRacket();
     // Reduce el tamaño de la raqueta del jugador si no ha alcanzado el límite mínimo.

   Problems:
     - Problema: Si la validación de los límites falla, la raqueta podría ser demasiado pequeña para el jugador.
*/

void halfRacket(){
    if (!(gameStateHandler->player.size.x <= screenWidth/(10*4))){
        gameStateHandler->player.size = (Vector2){ gameStateHandler->player.size.x/2, 10 };
    }
}

/* Function: addLife
   Descripción:
     Incrementa en uno el número de vidas del jugador si no ha alcanzado el máximo permitido.

   Params:
     (Ninguno)

   Returns:
     - void: No retorna valores.

   Restriction:
     - `gameStateHandler` debe estar correctamente inicializado.
     - El número de vidas del jugador debe estar dentro del rango permitido.

   Example:
     addLife();
     // Incrementa en uno la cantidad de vidas del jugador si es posible.

   Problems:
     - Problema: Si el número de vidas no está sincronizado con el estado del juego, podrían ocurrir inconsistencias.
*/

void addLife() {
    if (gameStateHandler->player.life < gameStateHandler->playerMaxLife ) {
        gameStateHandler->player.life ++;
    }
}

/* Function: speedUp
   Descripción:
     Duplica la velocidad de todas las bolas activas en el juego.

   Params:
     (Ninguno)

   Returns:
     - void: No retorna valores.

   Restriction:
     - `gameStateHandler` debe estar correctamente inicializado.
     - Solo las bolas activas verán afectada su velocidad.

   Example:
     speedUp();
     // Aumenta la velocidad de todas las bolas activas.

   Problems:
     - Problema: Si alguna bola tiene una velocidad nula, seguirá inactiva a pesar de la operación.
*/
void speedUp(){
    for (int i = 0; i < gameStateHandler->maxBalls; i++) {
        if (gameStateHandler->balls[i].active) {
            gameStateHandler->balls[i].speed.y *= 2;
            gameStateHandler->balls[i].speed.x *= 2;
        }
    }
}

/* Function: speedDown
Descripción:
Reduce a la mitad la velocidad de todas las bolas activas en el juego.

Params:
(Ninguno)

Returns:
- void: No retorna valores.

Restriction:
- `gameStateHandler` debe estar correctamente inicializado.
- Solo las bolas activas verán afectada su velocidad.

Example:
speedDown();
// Reduce la velocidad de todas las bolas activas.

Problems:
- Problema: Si alguna bola tiene una velocidad muy baja, podría hacerse imperceptible o no funcional.
*/
void speedDown(){
    for (int i = 0; i < gameStateHandler->maxBalls; i++) {
        if (gameStateHandler->balls[i].active) {
            gameStateHandler->balls[i].speed.y /= 2;
            gameStateHandler->balls[i].speed.x /= 2;
        }
    }
}


/* Function: update_brick_ball
   Descripción:
     Asigna el poder `ADD_BALL` a un ladrillo específico en las coordenadas `(i, j)`.

   Params:
     i - Índice de la fila del ladrillo.
     j - Índice de la columna del ladrillo.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `gameStateHandler` debe estar correctamente inicializado.
     - Los índices `(i, j)` deben estar dentro de los límites del tablero de ladrillos.

   Example:
     update_brick_ball(2, 3);
     // Asigna el poder `ADD_BALL` al ladrillo en la fila 2, columna 3.
*/

void update_brick_ball(int i, int j){
    gameStateHandler->bricks[i][j].power = ADD_BALL;
}

/* Function: update_brick_life
   Descripción:
     Asigna el poder `ADD_LIFE` a un ladrillo específico en las coordenadas `(i, j)`.

   Params:
     i - Índice de la fila del ladrillo.
     j - Índice de la columna del ladrillo.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `gameStateHandler` debe estar correctamente inicializado.
     - Los índices `(i, j)` deben estar dentro de los límites del tablero de ladrillos.

   Example:
     update_brick_life(1, 4);
     // Asigna el poder `ADD_LIFE` al ladrillo en la fila 1, columna 4.
*/
void update_brick_life(int i, int j){
    gameStateHandler->bricks[i][j].power = ADD_LIFE;
}

/* Function: update_brick_doubleRacket
   Descripción:
     Asigna el poder `DOUBLE_RACKET` a un ladrillo específico en las coordenadas `(i, j)`.

   Params:
     i - Índice de la fila del ladrillo.
     j - Índice de la columna del ladrillo.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `gameStateHandler` debe estar correctamente inicializado.
     - Los índices `(i, j)` deben estar dentro de los límites del tablero de ladrillos.

   Example:
     update_brick_doubleRacket(0, 2);
     // Asigna el poder `DOUBLE_RACKET` al ladrillo en la fila 0, columna 2.
*/
void update_brick_doubleRacket(int i, int j){
    gameStateHandler->bricks[i][j].power = DOUBLE_RACKET;
}

/* Function: update_brick_halfRacket
   Descripción:
     Asigna el poder `HALF_RACKET` a un ladrillo específico en las coordenadas `(i, j)`.

   Params:
     i - Índice de la fila del ladrillo.
     j - Índice de la columna del ladrillo.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `gameStateHandler` debe estar correctamente inicializado.
     - Los índices `(i, j)` deben estar dentro de los límites del tablero de ladrillos.

   Example:
     update_brick_halfRacket(3, 5);
     // Asigna el poder `HALF_RACKET` al ladrillo en la fila 3, columna 5.
*/
void update_brick_halfRacket(int i, int j){
    gameStateHandler->bricks[i][j].power = HALF_RACKET;
}

/* Function: update_brick_speedUp
   Descripción:
     Asigna el poder `SPEED_UP` a un ladrillo específico en las coordenadas `(i, j)`.

   Params:
     i - Índice de la fila del ladrillo.
     j - Índice de la columna del ladrillo.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `gameStateHandler` debe estar correctamente inicializado.
     - Los índices `(i, j)` deben estar dentro de los límites del tablero de ladrillos.

   Example:
     update_brick_speedUp(2, 1);
     // Asigna el poder `SPEED_UP` al ladrillo en la fila 2, columna 1.
*/

void update_brick_speedUp(int i, int j){
    gameStateHandler->bricks[i][j].power = SPEED_UP;
}

/* Function: update_brick_speedDown
   Descripción:
     Asigna el poder `SPEED_DOWN` a un ladrillo específico en las coordenadas `(i, j)`.

   Params:
     i - Índice de la fila del ladrillo.
     j - Índice de la columna del ladrillo.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `gameStateHandler` debe estar correctamente inicializado.
     - Los índices `(i, j)` deben estar dentro de los límites del tablero de ladrillos.

   Example:
     update_brick_speedDown(1, 3);
     // Asigna el poder `SPEED_DOWN` al ladrillo en la fila 1, columna 3.
*/
void update_brick_speedDown(int i, int j){
    gameStateHandler->bricks[i][j].power = SPEED_DOWN;
}

/* Function: update_player_score
Descripción:
Incrementa la puntuación del jugador en función de los puntos asignados al ladrillo destruido.

Params:
brickx - Índice de la fila del ladrillo destruido.
bricky - Índice de la columna del ladrillo destruido.

Returns:
- void: No retorna valores.

Restriction:
- `gameStateHandler` y el ladrillo en `(brickx, bricky)` deben estar correctamente inicializados.

Example:
update_player_score(0, 2);
// Incrementa la puntuación del jugador según los puntos del ladrillo en la fila 0, columna 2.
*/

void update_player_score(int brickx, int bricky) {
    updatePlayerScore(&gameStateHandler->player, gameStateHandler->bricks[brickx][bricky].points);
}

/* Function: check_brick
Descripción:
Verifica el tipo de poder asociado al ladrillo en las coordenadas `(i, j)` y ejecuta la acción correspondiente.
Las acciones incluyen agregar bolas, cambiar el tamaño de la raqueta, ajustar la velocidad de las bolas o
incrementar vidas del jugador.

Params:
i - Índice de la fila del ladrillo.
j - Índice de la columna del ladrillo.

Returns:
- void: No retorna valores.

Restriction:
- `gameStateHandler` debe estar correctamente inicializado.
- Los índices `(i, j)` deben estar dentro de los límites válidos del tablero de ladrillos.

Example:
check_brick(2, 3);
// Verifica y ejecuta el poder asociado al ladrillo en la fila 2, columna 3.

Problems:
- Problema: Si `gameStateHandler` es `NULL` o si los índices están fuera de rango, puede haber errores.
- Solución: Validar el estado del juego y los índices antes de procesar.
- Problema: Si el poder no está correctamente definido, no se ejecutará ninguna acción.
- Solución: Garantizar que cada ladrillo tenga un poder válido asignado.

References:
- Ninguna referencia externa específica.
*/

void check_brick(int i, int j){
    if (gameStateHandler->bricks[i][j].power == ADD_BALL) {
        add_ball(gameStateHandler->bricks[i][j].position.x, gameStateHandler->bricks[i][j].position.y);
    } else if (gameStateHandler->bricks[i][j].power == DOUBLE_RACKET) {
        doubleRacket();
    }
    else if (gameStateHandler->bricks[i][j].power == ADD_LIFE) {
        addLife();
    } else if (gameStateHandler->bricks[i][j].power == HALF_RACKET) {
        halfRacket();
    } else if (gameStateHandler->bricks[i][j].power == SPEED_UP) {
        speedUp();
    } else if (gameStateHandler->bricks[i][j].power == SPEED_DOWN) {
        speedDown();
    }
}