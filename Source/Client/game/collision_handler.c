#include "collision_handler.h"
#include <raylib.h>
#include <math.h>
#include "powerHandler.h"

// Función para manejar las colisiones de las bolas con las paredes
void handle_ball_wall_collision(GameState *gameState, float screenWidth, float screenHeight) {

    for (int i = 0; i < MAX_BALLS; i++) {
        Ball *ball = &gameState->balls[i]; // Access the ball directly from gameState
        if (ball->active) {
            // Check for wall collisions
            if ((ball->position.x + ball->radius) >= screenWidth || (ball->position.x - ball->radius) <= 0) {
                ball->speed.x *= -1; // Reverse horizontal speed
            }
            if ((ball->position.y - ball->radius) <= 0) {
                ball->speed.y *= -1; // Reverse vertical speed
            }
            if ((ball->position.y + ball->radius) >= screenHeight) {
                ball->speed = (Vector2) {0, 0}; // Stop the ball
                ball->active = false; // Deactivate the ball
                // Reduce player's life since the ball has fallen off the screen
                gameState->player.life--;
            }

        }

    }

}

// Función para manejar la colisión de una bola con el jugador
void handle_ball_player_collision(GameState *gameStatus) {
    for (int i = 0; i < MAX_BALLS; i++) {
        // Lógica de Colisión: bola vs jugador
        if (CheckCollisionCircleRec(gameStatus->balls[i].position, gameStatus->balls[i].radius,
                                    (Rectangle) {gameStatus->player.position.x - gameStatus->player.size.x / 2,
                                                 gameStatus->player.position.y - gameStatus->player.size.y / 2,
                                                 gameStatus->player.size.x, gameStatus->player.size.y})) {
            if (gameStatus->balls[i].speed.y > 0) {
                gameStatus->balls[i].speed.y *= -1;
                gameStatus->balls[i].speed.x = (gameStatus->balls[i].position.x - gameStatus->player.position.x) /
                                              (gameStatus->player.size.x / 2) * 5;
            }
                                                 }
    }
}

// Función para manejar la colisión de una bola con los ladrillos
void handle_ball_brick_collision(GameState *gameState) {
    // Collision logic: ball vs bricks
    for (int i = 0; i < LINES_OF_BRICKS; i++) {
        for (int j = 0; j < BRICKS_PER_LINE; j++) {
            if (gameState->bricks[i][j].active) {
                for (int b = 0; b < MAX_BALLS; b++) {
                    Ball *ball = &gameState->balls[b];

                    if (ball->active) {
                        // Hit below
                        if (((ball->position.y - ball->radius) <=
                             (gameState->bricks[i][j].position.y + gameState->brickSize.y / 2)) &&
                            ((ball->position.y - ball->radius) >
                             (gameState->bricks[i][j].position.y + gameState->brickSize.y / 2 + ball->speed.y)) &&
                            ((fabs(ball->position.x - gameState->bricks[i][j].position.x)) <
                             (gameState->brickSize.x / 2 + ball->radius * 2 / 3)) && (ball->speed.y < 0)) {
                            gameState->bricks[i][j].active = false;
                            ball->speed.y *= -1;

                            update_player_score(i, j);
                            check_brick(i, j);
                        }
                        // Hit above
                        else if (((ball->position.y + ball->radius) >=
                                  (gameState->bricks[i][j].position.y - gameState->brickSize.y / 2)) &&
                                 ((ball->position.y + ball->radius) <
                                  (gameState->bricks[i][j].position.y - gameState->brickSize.y / 2 + ball->speed.y)) &&
                                 ((fabs(ball->position.x - gameState->bricks[i][j].position.x)) <
                                  (gameState->brickSize.x / 2 + ball->radius * 2 / 3)) && (ball->speed.y > 0)) {
                            gameState->bricks[i][j].active = false;
                            ball->speed.y *= -1;

                            update_player_score(i, j);
                            check_brick(i, j);
                        }
                        // Hit left
                        else if (((ball->position.x + ball->radius) >=
                                  (gameState->bricks[i][j].position.x - gameState->brickSize.x / 2)) &&
                                 ((ball->position.x + ball->radius) <
                                  (gameState->bricks[i][j].position.x - gameState->brickSize.x / 2 + ball->speed.x)) &&
                                 ((fabs(ball->position.y - gameState->bricks[i][j].position.y)) <
                                  (gameState->brickSize.y / 2 + ball->radius * 2 / 3)) && (ball->speed.x > 0)) {
                            gameState->bricks[i][j].active = false;
                            ball->speed.x *= -1;

                            update_player_score(i, j);
                            check_brick(i, j);
                        }
                        // Hit right
                        else if (((ball->position.x - ball->radius) <=
                                  (gameState->bricks[i][j].position.x + gameState->brickSize.x / 2)) &&
                                 ((ball->position.x - ball->radius) >
                                  (gameState->bricks[i][j].position.x + gameState->brickSize.x / 2 + ball->speed.x)) &&
                                 ((fabs(ball->position.y - gameState->bricks[i][j].position.y)) <
                                  (gameState->brickSize.y / 2 + ball->radius * 2 / 3)) && (ball->speed.x < 0)) {
                            gameState->bricks[i][j].active = false;
                            ball->speed.x *= -1;

                            update_player_score(i, j);
                            check_brick(i, j);
                        }
                    }
                }
            }
        }
    }
}
// Función principal para manejar todas las colisiones
void handle_collisions(GameState *gameState) {
    handle_ball_wall_collision(gameState, screenWidth, screenHeight);
    handle_ball_player_collision(gameState);
    handle_ball_brick_collision(gameState);
}


