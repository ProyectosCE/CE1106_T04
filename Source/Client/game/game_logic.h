#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "../game_status.h"


#define LINES_OF_BRICKS         8
#define BRICKS_PER_LINE        8
#define MAX_BALLS              5

void update_game(GameState* gameState);
void sendGameState(GameState *gameState);

#endif // GAME_LOGIC_H
