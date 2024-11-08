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
#ifndef BRICK_H
#define BRICK_H

#include <raylib.h>

#include "../../game_status.h"

void init_bricks(GameState* gameState, Vector2 brickSize);
void deactivate_brick(Brick* brick);




#endif // BRICK_H
