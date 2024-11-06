#ifndef BALL_H
#define BALL_H

#include <raylib.h>
#include "../../game_status.h"
#include <stdbool.h>

#include "player.h"



#define MAX_BALLS              5

void init_balls(Ball* ball);
void activate_ball(Ball* ball, Vector2 position, Vector2 speed);
void update_ball_positions(Player *player, Ball *balls);

#endif // BALL_H
