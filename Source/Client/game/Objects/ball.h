#ifndef BALL_H
#define BALL_H

#include <raylib.h>
#include "../../game_status.h"
#include <stdbool.h>

#include "player.h"





void init_balls(Ball* ball);
void activate_ball(Ball* ball, Vector2 position, Vector2 speed);
void update_ball_positions(Player *player, Ball *balls);

bool noBallsActive(Ball *balls, int maxBalls);

#endif // BALL_H
