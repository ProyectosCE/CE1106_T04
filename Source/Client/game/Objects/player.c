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

#include "player.h"


/* Function: init_player
   Descripción:
     Inicializa los parámetros básicos del jugador, incluyendo tamaño, posición inicial, vidas máximas,
     puntaje, y estados de bonificación.

   Params:
     player - Puntero a la estructura `Player` que será inicializada.
     maxLife - Número máximo de vidas para el jugador.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `player` debe ser un puntero válido.
     - `screenWidth` y `screenHeight` deben estar definidos y ser valores positivos.

   Example:
     Player player;
     init_player(&player, 3);
     // Inicializa un jugador con 3 vidas.

   Problems:
     - Problema: Si `player` es nulo, la inicialización no se realizará.
       - Solución: Validar el puntero antes de proceder.

   References:
     - Ninguna referencia externa específica.
*/
void init_player(Player* player, int maxLife) {

    player->size = (Vector2){ screenWidth/10, 10 };
    player->position = (Vector2){ screenWidth/2, screenHeight-player->size.y};
    player->life = maxLife;
    player->score = 0;
    player->doubleRacket = false;
    player->halfRacket = false;
}

/* Function: updatePlayerScore
   Descripción:
     Incrementa el puntaje del jugador en función de los puntos obtenidos.

   Params:
     player - Puntero a la estructura `Player` que será actualizada.
     points - Puntos a añadir al puntaje actual del jugador.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `player` debe ser un puntero válido.

   Example:
     updatePlayerScore(&player, 200);
     // Aumenta el puntaje del jugador en 200 puntos.

   Problems:
     - Problema: Si `player` es nulo, no se actualizará el puntaje.
       - Solución: Validar el puntero antes de proceder.

   References:
     - Ninguna referencia externa específica.
*/
void updatePlayerScore(Player* player, const int points) {
    player->score += points;
    //printf("Score: %d\n", player->score);
}

/* Function: double_racket
   Descripción:
     Activa la bonificación de duplicar el tamaño de la raqueta del jugador.

   Params:
     player - Puntero a la estructura `Player` que será modificada.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `player` debe ser un puntero válido.

   Example:
     double_racket(&player);
     // Activa la bonificación de raqueta doble.

   Problems:
     - Problema: Si `player` es nulo, no se modificará su estado.
       - Solución: Validar el puntero antes de proceder.

   References:
     - Ninguna referencia externa específica.
*/
void double_racket(Player* player) {
    player->doubleRacket = true;
}

/* Function: half_racket
   Descripción:
     Activa la bonificación que reduce a la mitad el tamaño de la raqueta del jugador.

   Params:
     player - Puntero a la estructura `Player` que será modificada.

   Returns:
     - void: No retorna valores.

   Restriction:
     - `player` debe ser un puntero válido.

   Example:
     half_racket(&player);
     // Activa la bonificación de raqueta reducida.

   Problems:
     - Problema: Si `player` es nulo, no se modificará su estado.
       - Solución: Validar el puntero antes de proceder.

   References:
     - Ninguna referencia externa específica.
*/
void half_racket(Player* player) {
    player->halfRacket = true;
}


/* Function: update_player_movement
Descripción:
Actualiza la posición del jugador en función de la entrada del teclado. El movimiento
está limitado al rango de la pantalla.

Params:
player - Puntero a la estructura `Player` que será movida.
screenWidth - Ancho de la pantalla para limitar el movimiento.

Returns:
- void: No retorna valores.

Restriction:
- `player` debe ser un puntero válido.
- `screenWidth` debe ser mayor a cero.

Example:
update_player_movement(&player, screenWidth);
// Mueve al jugador en la dirección indicada por las teclas.

Problems:
- Problema: Si `player` es nulo, no se actualizará la posición.
- Solución: Validar el puntero antes de proceder.
- Problema: Si `screenWidth` es incorrecto, puede causar límites de movimiento no deseados.
- Solución: Validar el valor antes de proceder.

References:
- Ninguna referencia externa específica.
*/

void update_player_movement(Player *player, float screenWidth) {
    if (IsKeyDown(KEY_A)) player->position.x -= 5;
    if (IsKeyDown(KEY_D)) player->position.x += 5;

    // Limitar el movimiento del jugador a la pantalla
    if ((player->position.x - player->size.x / 2) <= 0) player->position.x = player->size.x / 2;
    if ((player->position.x + player->size.x / 2) >= screenWidth) player->position.x = screenWidth - player->size.x / 2;
}