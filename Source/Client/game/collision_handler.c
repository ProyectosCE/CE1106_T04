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


//BIBLIOTECAS EXTRERNAS
#include <raylib.h>
#include <math.h>
// BIBLIOTECAS DE PROYECTO
#include "collision_handler.h"
#include "powerHandler.h"

/* Function: handle_ball_wall_collision
Descripción:
Gestiona las colisiones de las bolas activas con las paredes de la pantalla. Invierte la dirección de
la velocidad de la bola cuando colisiona con los bordes horizontales o el borde superior. Detiene y
desactiva la bola si colisiona con el borde inferior.

Params:
gameState - Puntero al estado global del juego que contiene las bolas y su información.
screenWidth - Ancho de la pantalla.
screenHeight - Alto de la pantalla.

Returns:
- void: No retorna valores.

Restriction:
- `gameState` debe estar correctamente inicializado con información válida sobre las bolas.
- `screenWidth` y `screenHeight` deben reflejar las dimensiones actuales de la pantalla.

Example:
handle_ball_wall_collision(&gameState, 800, 600);
// Maneja las colisiones de las bolas con las paredes en una pantalla de 800x600.

Problems:
- Problema: Si `screenWidth` o `screenHeight` no corresponden a las dimensiones reales de la pantalla,
las colisiones podrían comportarse de manera incorrecta.
- Solución: Validar las dimensiones antes de llamar a la función.

References:
- Ninguna referencia externa específica.
*/
void handle_ball_wall_collision(GameState *gameState, float screenWidth, float screenHeight) {

    for (int i = 0; i < gameState->maxBalls; i++) {
        Ball *ball = &gameState->balls[i]; // Access the ball directly from gameState
        if (ball->active) {
            // Check for wall collisions
            if ((ball->position.x + ball->radius) >= screenWidth || (ball->position.x - ball->radius) <= 0) {
                ball->speed.x *= -1; // Reverse horizontal speed
            }
            if ((ball->position.y - ball->radius) <= 0) {
                ball->speed.y *= -1; // Reverse vertical speed
            }
            if ((ball->position.y + ball->radius) >= screenHeight) {
                ball->speed = (Vector2) {0, 0}; // Stop the ball
                ball->active = false; // Deactivate the ball
            }

        }

    }

}


/* Function: handle_ball_player_collision
Descripción:
Gestiona la colisión entre las bolas activas y el jugador (raqueta). Si una bola colisiona con la raqueta,
invierte su dirección vertical y ajusta su velocidad horizontal en función de la posición relativa del impacto.

Params:
gameStatus - Puntero al estado global del juego que contiene la información de las bolas y el jugador.

Returns:
- void: No retorna valores.

Restriction:
- `gameStatus` debe estar correctamente inicializado con las posiciones y tamaños del jugador y las bolas.

Example:
handle_ball_player_collision(&gameStatus);
// Maneja las colisiones entre las bolas activas y la raqueta del jugador.

Problems:
- Problema: Si las posiciones o dimensiones de los objetos no están correctamente configuradas,
la detección de colisión puede fallar o generar resultados incorrectos.
- Solución: Validar las propiedades del jugador y las bolas antes de llamar a esta función.

References:
- CheckCollisionCircleRec: https://www.raylib.com
*/

void handle_ball_player_collision(GameState *gameStatus) {
    for (int i = 0; i < gameStatus->maxBalls; i++) {
        // Lógica de Colisión: bola vs jugador
        if (CheckCollisionCircleRec(gameStatus->balls[i].position, gameStatus->balls[i].radius,
                                    (Rectangle) {gameStatus->player.position.x - gameStatus->player.size.x / 2,
                                                 gameStatus->player.position.y - gameStatus->player.size.y / 2,
                                                 gameStatus->player.size.x, gameStatus->player.size.y})) {
            if (gameStatus->balls[i].speed.y > 0) {
                gameStatus->balls[i].speed.y *= -1;
                gameStatus->balls[i].speed.x = (gameStatus->balls[i].position.x - gameStatus->player.position.x) /
                                              (gameStatus->player.size.x / 2) * 5;
            }
                                                 }
    }
}

/* Function: handle_ball_brick_collision
Descripción:
Gestiona las colisiones entre las bolas activas y los ladrillos. Al detectar una colisión,
desactiva el ladrillo, invierte la dirección de la velocidad de la bola, y actualiza la puntuación
del jugador.

Params:
gameState - Puntero al estado global del juego (`GameState *`) que contiene la información de las bolas,
ladrillos y su estado actual.

Returns:
- void: No retorna valores.

Restriction:
- `gameState` debe estar correctamente inicializado con posiciones, tamaños y estados válidos
de los ladrillos y bolas.

Example:
handle_ball_brick_collision(&gameState);
// Gestiona las colisiones de las bolas con los ladrillos activos.

Problems:
- Problema: Si `gameState` no está completamente inicializado, las detecciones de colisión
podrían fallar o generar resultados inconsistentes.
- Solución: Validar previamente el estado del juego.
- Problema: Puede haber redundancia en los bloques de código de colisión (e.g., para arriba, abajo, izquierda, derecha).
- Solución: Simplificar la lógica de colisión utilizando funciones auxiliares reutilizables.

References:
- Ninguna referencia externa específica.
*/

void handle_ball_brick_collision(GameState *gameState) {
    // Collision logic: ball vs bricks
    for (int i = 0; i < gameState->linesOfBricks; i++) {
        for (int j = 0; j < gameState->bricksPerLine; j++) {
            if (gameState->bricks[i][j].active) {
                for (int b = 0; b < gameState->maxBalls; b++) {
                    Ball *ball = &gameState->balls[b];

                    if (ball->active) {
                        // Hit below
                        if (((ball->position.y - ball->radius) <=
                             (gameState->bricks[i][j].position.y + gameState->brickSize.y / 2)) &&
                            ((ball->position.y - ball->radius) >
                             (gameState->bricks[i][j].position.y + gameState->brickSize.y / 2 + ball->speed.y)) &&
                            ((fabs(ball->position.x - gameState->bricks[i][j].position.x)) <
                             (gameState->brickSize.x / 2 + ball->radius * 2 / 3)) && (ball->speed.y < 0)) {
                            gameState->bricks[i][j].active = false;
                            ball->speed.y *= -1;

                            update_player_score(i, j);
                            check_brick(i, j);
                        }
                        // Hit above
                        else if (((ball->position.y + ball->radius) >=
                                  (gameState->bricks[i][j].position.y - gameState->brickSize.y / 2)) &&
                                 ((ball->position.y + ball->radius) <
                                  (gameState->bricks[i][j].position.y - gameState->brickSize.y / 2 + ball->speed.y)) &&
                                 ((fabs(ball->position.x - gameState->bricks[i][j].position.x)) <
                                  (gameState->brickSize.x / 2 + ball->radius * 2 / 3)) && (ball->speed.y > 0)) {
                            gameState->bricks[i][j].active = false;
                            ball->speed.y *= -1;

                            update_player_score(i, j);
                            check_brick(i, j);
                        }
                        // Hit left
                        else if (((ball->position.x + ball->radius) >=
                                  (gameState->bricks[i][j].position.x - gameState->brickSize.x / 2)) &&
                                 ((ball->position.x + ball->radius) <
                                  (gameState->bricks[i][j].position.x - gameState->brickSize.x / 2 + ball->speed.x)) &&
                                 ((fabs(ball->position.y - gameState->bricks[i][j].position.y)) <
                                  (gameState->brickSize.y / 2 + ball->radius * 2 / 3)) && (ball->speed.x > 0)) {
                            gameState->bricks[i][j].active = false;
                            ball->speed.x *= -1;

                            update_player_score(i, j);
                            check_brick(i, j);
                        }
                        // Hit right
                        else if (((ball->position.x - ball->radius) <=
                                  (gameState->bricks[i][j].position.x + gameState->brickSize.x / 2)) &&
                                 ((ball->position.x - ball->radius) >
                                  (gameState->bricks[i][j].position.x + gameState->brickSize.x / 2 + ball->speed.x)) &&
                                 ((fabs(ball->position.y - gameState->bricks[i][j].position.y)) <
                                  (gameState->brickSize.y / 2 + ball->radius * 2 / 3)) && (ball->speed.x < 0)) {
                            gameState->bricks[i][j].active = false;
                            ball->speed.x *= -1;

                            update_player_score(i, j);
                            check_brick(i, j);
                        }
                    }
                }
            }
        }
    }
}
/* Function: handle_collisions
   Descripción:
     Gestiona todas las colisiones en el juego, incluyendo las colisiones de las bolas con las paredes,
     el jugador y los ladrillos.

   Params:
     gameState - Puntero al estado global del juego (`GameState *`) que contiene la información necesaria
                 para procesar las colisiones.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `gameState` debe estar correctamente inicializado antes de llamar a esta función.
     - Las funciones auxiliares como `handle_ball_wall_collision`, `handle_ball_player_collision`, y
       `handle_ball_brick_collision` deben estar correctamente implementadas.

   Example:
     handle_collisions(&gameState);
     // Llama a esta función en el bucle principal para manejar las colisiones del juego.

   Problems:
     - Problema: Si alguna función auxiliar no está correctamente implementada, puede generar inconsistencias
       en las colisiones.
       - Solución: Asegurar pruebas independientes de cada función auxiliar.
*/
void handle_collisions(GameState *gameState) {
    handle_ball_wall_collision(gameState, screenWidth, screenHeight);
    handle_ball_player_collision(gameState);
    handle_ball_brick_collision(gameState);
}

/* Function: bloquesEliminados
Descripción:
Verifica si todos los ladrillos del juego han sido eliminados, es decir, están inactivos.

Params:
gameState - Puntero al estado global del juego (`GameState *`) que contiene la información de los ladrillos.

Returns:
- bool: `true` si todos los ladrillos están inactivos, `false` si al menos uno está activo.

Restriction:
- `gameState` debe estar correctamente inicializado con los estados actuales de los ladrillos.

Example:
if (bloquesEliminados(&gameState)) {
printf("Todos los ladrillos han sido eliminados.\n");
}

Problems:
- Problema: Si la información sobre los ladrillos no está sincronizada con el estado real del juego,
el resultado puede ser incorrecto.
- Solución: Garantizar que los estados de los ladrillos se actualicen correctamente.
*/
bool bloquesEliminados(GameState* gameState) {
    for (int i = 0; i < gameState->linesOfBricks; i++) {
        for (int j = 0; j < gameState->bricksPerLine; j++) {
            if (gameState->bricks[i][j].active) {
                return false; // Hay al menos un ladrillo activo
            }
        }
    }
    return true; // Todos los ladrillos están inactivos
}

