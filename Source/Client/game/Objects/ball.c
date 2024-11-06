#include "ball.h"

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

void activate_ball(Ball* ball, const Vector2 position, const Vector2 speed) {
    ball->position = position;
    ball->speed = speed;
    ball->active = true;
}


// Comprobar si no hay bolas activas
bool noBallsActive(Ball *balls, int maxBalls) {
    for (int i = 0; i < maxBalls; i++) {
        if (balls[i].active) {
            return false; // Hay al menos una bola activa
        }
    }
    return true; // No hay ninguna bola activa
}

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