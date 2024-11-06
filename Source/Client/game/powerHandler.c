#include "powerHandler.h"
#include "../game_status.h"
#include "Objects/player.h"

GameState *gameStateHandler;

void initPowerHandler() {
    gameStateHandler = getGameState();
}

void check_brick(int i, int j){
    if (gameStateHandler->bricks[i][j].add_ball) {
        add_ball(gameStateHandler->bricks[i][j].position.x, gameStateHandler->bricks[i][j].position.y);
    } else if (gameStateHandler->bricks[i][j].add_life) {
        add_life(&gameStateHandler->player);
    } else if (gameStateHandler->bricks[i][j].add_doubleRacket) {
        double_racket(&gameStateHandler->player);
    } else if (gameStateHandler->bricks[i][j].add_halfRacket) {
        half_racket(&gameStateHandler->player);
    } else if (gameStateHandler->bricks[i][j].speedUp) {
        speedUp();
    } else if (gameStateHandler->bricks[i][j].speedDown) {
        speedDown();
    }
}

void update_brick_score(int level, int new_points) {
    for (int i = 0; i < LINES_OF_BRICKS; i++) {
        for (int j = 0; j < BRICKS_PER_LINE; j++) {
            if (gameStateHandler->bricks[i][j].active) {
                // Actualizar puntos únicamente en el nivel especificado
                if ((level == 4 && i < 2) ||
                    (level == 3 && i >= 2 && i < 4) ||
                    (level == 2 && i >= 4 && i < 6) ||
                    (level == 1 && i >= 6)) {

                    gameStateHandler->bricks[i][j].points = new_points;
                }
            }
        }
    }
}



void add_ball(int posX, int posY) {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (!gameStateHandler->balls[i].active) {
            gameStateHandler->balls[i].active = true;
            gameStateHandler->balls[i].position = (Vector2){ posX, posY };  // Posición del ladrillo destruido
            gameStateHandler->balls[i].speed = (Vector2){0, -5};
            break;  // Salir del bucle una vez que activamos una nueva bola
        }
    }
}

void doubleRacket(){
    gameStateHandler->player.size = (Vector2){ screenWidth/5, 10 };
}
void halfRacket(){
    gameStateHandler->player.size = (Vector2){ screenWidth/20, 10 };
}
void speedUp(){
    for (int i = 0; i < MAX_BALLS; i++) {
        if (gameStateHandler->balls[i].active) {
            gameStateHandler->balls[i].speed.y *= 2;
            gameStateHandler->balls[i].speed.x *= 2;
        }
    }
}
void speedDown(){
    for (int i = 0; i < MAX_BALLS; i++) {
        if (gameStateHandler->balls[i].active) {
            gameStateHandler->balls[i].speed.y /= 2;
            gameStateHandler->balls[i].speed.x /= 2;
        }
    }
}


void update_brick_ball(int i, int j){
    gameStateHandler->bricks[i][j].add_ball = true;
}

void update_brick_life(int i, int j){
    gameStateHandler->bricks[i][j].add_life = true;
}

void update_brick_doubleRacket(int i, int j){
    gameStateHandler->bricks[i][j].add_doubleRacket = true;
}

void update_brick_halfRacket(int i, int j){
    gameStateHandler->bricks[i][j].add_halfRacket = true;
}

void update_brick_speedUp(int i, int j){
    gameStateHandler->bricks[i][j].speedUp = true;
}

void update_brick_speedDown(int i, int j){
    gameStateHandler->bricks[i][j].speedDown = true;
}

void update_player_score(int brickx, int bricky) {
    updatePlayerScore(&gameStateHandler->player, gameStateHandler->bricks[brickx][bricky].points);
}