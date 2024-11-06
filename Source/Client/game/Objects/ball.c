#include "ball.h"

void init_balls(Ball* ball) {
    for (int i = 0; i < MAX_BALLS; i++) {
        ball[i].position = (Vector2){ screenWidth/2, screenHeight*7/8 };
        ball[i].speed = (Vector2){ 0, 0 };
        ball[i].radius = 7;
        ball[i].active = false;
    }
}

void activate_ball(Ball* ball, const Vector2 position, const Vector2 speed) {
    ball->position = position;
    ball->speed = speed;
    ball->active = true;
}


void update_ball_positions(Player *player, Ball *balls) {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (balls[i].active) {
            balls[i].position.x += balls[i].speed.x;
            balls[i].position.y += balls[i].speed.y;
        } else {
            balls[i].position = (Vector2){player->position.x, screenHeight * 7 / 8 - 30};
        }
    }

    // Lógica de lanzamiento de la primera bola
    if (!balls[0].active && IsKeyPressed(KEY_W)) {
        balls[0].active = true;
        balls[0].speed = (Vector2){0, -5};
    }
}