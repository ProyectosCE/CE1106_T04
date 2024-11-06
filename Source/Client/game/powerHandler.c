#include "powerHandler.h"
#include "../game_status.h"
#include "Objects/player.h"

GameState *gameStateHandler;

void initPowerHandler() {
    gameStateHandler = getGameState();
}



void update_brick_score(int level, int new_points) {
    level++;
    new_points++;
    for (int i = 0; i < gameStateHandler->linesOfBricks; i++) {
        for (int j = 0; j < gameStateHandler->bricksPerLine; j++) {
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
    for (int i = 0; i < gameStateHandler->maxBalls; i++) {
        if (!gameStateHandler->balls[i].active) {
            gameStateHandler->balls[i].active = true;
            gameStateHandler->balls[i].position = (Vector2){ posX, posY };  // Posición del ladrillo destruido
            gameStateHandler->balls[i].speed = (Vector2){0, 5};
            break;  // Salir del bucle una vez que activamos una nueva bola
        }
    }
}

void doubleRacket(){
    if (! (gameStateHandler->player.size.x >= 4*(screenWidth/10))) {
        gameStateHandler->player.size = (Vector2){ gameStateHandler->player.size.x*2, 10 };
    }
}
void halfRacket(){
    if (!(gameStateHandler->player.size.x <= screenWidth/(10*4))){
        gameStateHandler->player.size = (Vector2){ gameStateHandler->player.size.x/2, 10 };
    }
}

void addLife() {
    if (gameStateHandler->player.life < gameStateHandler->playerMaxLife ) {
        gameStateHandler->player.life ++;
    }

}
void speedUp(){
    for (int i = 0; i < gameStateHandler->maxBalls; i++) {
        if (gameStateHandler->balls[i].active) {
            gameStateHandler->balls[i].speed.y *= 2;
            gameStateHandler->balls[i].speed.x *= 2;
        }
    }
}
void speedDown(){
    for (int i = 0; i < gameStateHandler->maxBalls; i++) {
        if (gameStateHandler->balls[i].active) {
            gameStateHandler->balls[i].speed.y /= 2;
            gameStateHandler->balls[i].speed.x /= 2;
        }
    }
}


void update_brick_ball(int i, int j){
    gameStateHandler->bricks[i][j].power = ADD_BALL;
}

void update_brick_life(int i, int j){
    gameStateHandler->bricks[i][j].power = ADD_LIFE;
}

void update_brick_doubleRacket(int i, int j){
    gameStateHandler->bricks[i][j].power = DOUBLE_RACKET;
}

void update_brick_halfRacket(int i, int j){
    gameStateHandler->bricks[i][j].power = HALF_RACKET;
}

void update_brick_speedUp(int i, int j){
    gameStateHandler->bricks[i][j].power = SPEED_UP;
}

void update_brick_speedDown(int i, int j){
    gameStateHandler->bricks[i][j].power = SPEED_DOWN;
}

void update_player_score(int brickx, int bricky) {
    updatePlayerScore(&gameStateHandler->player, gameStateHandler->bricks[brickx][bricky].points);
}

void check_brick(int i, int j){
    if (gameStateHandler->bricks[i][j].power == ADD_BALL) {
        add_ball(gameStateHandler->bricks[i][j].position.x, gameStateHandler->bricks[i][j].position.y);
    } else if (gameStateHandler->bricks[i][j].power == DOUBLE_RACKET) {
        doubleRacket();
    }
    else if (gameStateHandler->bricks[i][j].power == ADD_LIFE) {
        addLife();
    } else if (gameStateHandler->bricks[i][j].power == HALF_RACKET) {
        halfRacket();
    } else if (gameStateHandler->bricks[i][j].power == SPEED_UP) {
        speedUp();
    } else if (gameStateHandler->bricks[i][j].power == SPEED_DOWN) {
        speedDown();
    }
}