#ifndef PLAYER_H
#define PLAYER_H

#include "../../game_status.h"




void init_player(Player* player, int maxLife);
void updatePlayerScore(Player* player, int points);
void add_life(Player* player);
void double_racket(Player* player);
void half_racket(Player* player);
void update_player_movement(Player *player, float screenWidth);

#endif // PLAYER_H
