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

#include "ball.h"


/* Function: init_balls
   Descripción:
     Inicializa las propiedades de todas las bolas en el arreglo `Ball`. Configura sus posiciones iniciales,
     velocidad en cero, radio, y estado inactivo.

   Params:
     ball - Puntero al arreglo de bolas que será inicializado.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `ball` debe ser un puntero válido a un arreglo de bolas.
     - Se asume que `game_state->maxBalls` está configurado correctamente.

   Example:
     Ball balls[MAX_BALLS];
     init_balls(balls);
     // Inicializa las bolas para el juego.

   Problems:
     - Problema: Si `game_state` no está inicializado, la función fallará.
       - Solución: Validar `game_state` antes de proceder.

   References:
     - Ninguna referencia externa específica.
*/
void init_balls(Ball* ball) {
    GameState *game_state = getGameState();
    for (int i = 0; i < game_state->maxBalls; i++) {
        ball[i].position = (Vector2){ screenWidth/2, screenHeight*7/8 };
        ball[i].speed = (Vector2){ 0, 0 };
        ball[i].radius = 7;
        ball[i].active = false;
    }
    game_state->bolaLanzada=false;
}



/* Function: activate_ball
   Descripción:
     Activa una bola específica en el juego, configurando su posición, velocidad y estado activo.

   Params:
     ball - Puntero a la bola que será activada.
     position - Posición inicial de la bola.
     speed - Velocidad inicial de la bola.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `ball` debe ser un puntero válido a una estructura `Ball`.

   Example:
     activate_ball(&balls[0], (Vector2){100, 200}, (Vector2){2, -3});
     // Activa la primera bola con posición y velocidad iniciales.

   Problems:
     - Problema: Si `ball` es nulo, no se podrá realizar la activación.
       - Solución: Validar el puntero antes de proceder.

   References:
     - Ninguna referencia externa específica.
*/
void activate_ball(Ball* ball, const Vector2 position, const Vector2 speed) {
    ball->position = position;
    ball->speed = speed;
    ball->active = true;
}


/* Function: noBallsActive
   Descripción:
     Verifica si todas las bolas en el arreglo están inactivas.

   Params:
     balls - Puntero al arreglo de bolas.
     maxBalls - Número máximo de bolas en el arreglo.

   Returns:
     - bool: `true` si no hay bolas activas, `false` si al menos una está activa.

   Restriction:
     - `balls` debe ser un puntero válido.

   Example:
     if (noBallsActive(balls, MAX_BALLS)) {
         printf("No hay bolas activas en el juego.\n");
     }

   Problems:
     - Problema: Si `balls` es nulo, puede causar un fallo.
       - Solución: Validar el puntero antes de proceder.

   References:
     - Ninguna referencia externa específica.
*/

bool noBallsActive(Ball *balls, int maxBalls) {
    for (int i = 0; i < maxBalls; i++) {
        if (balls[i].active) {
            return false; // Hay al menos una bola activa
        }
    }
    return true; // No hay ninguna bola activa
}


/* Function: update_ball_positions
Descripción:
Actualiza las posiciones de las bolas activas según su velocidad. Si la bola está inactiva,
la posiciona en la raqueta del jugador. También permite el lanzamiento de la primera bola al presionar una tecla.

Params:
player - Puntero a la estructura del jugador que sirve de referencia para las bolas inactivas.
balls - Puntero al arreglo de bolas que será actualizado.

Returns:
- void: No retorna valores.

Restriction:
- `player` y `balls` deben ser punteros válidos.
- Se asume que `KEY_W` está configurado para el lanzamiento de la bola.

Example:
update_ball_positions(&player, balls);
// Actualiza las posiciones de las bolas en el juego.

Problems:
- Problema: Si `game_state` no está inicializado, no se podrá realizar la actualización.
- Solución: Validar `game_state` antes de proceder.

References:
- Ninguna referencia externa específica.
*/

void update_ball_positions(Player *player, Ball *balls) {
    GameState *game_state = getGameState();
    for (int i = 0; i < game_state->maxBalls; i++) {
        if (balls[i].active) {
            balls[i].position.x += balls[i].speed.x;
            balls[i].position.y += balls[i].speed.y;
        } else {
            balls[i].position = (Vector2){player->position.x, screenHeight * 7 / 8 - 30};
        }
    }

    // Lógica de lanzamiento de la primera bola
    if (noBallsActive(balls, game_state->maxBalls) && IsKeyPressed(KEY_W)) {
        balls[0].active = true;
        balls[0].speed = (Vector2){0, -5};
        game_state->bolaLanzada=true;
    }
}