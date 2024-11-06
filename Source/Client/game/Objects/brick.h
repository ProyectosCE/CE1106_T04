#ifndef BRICK_H
#define BRICK_H

#include <raylib.h>

#include "../../game_status.h"


void init_bricks(GameState* gameState, Vector2 brickSize);
void deactivate_brick(Brick* brick);




#endif // BRICK_H
