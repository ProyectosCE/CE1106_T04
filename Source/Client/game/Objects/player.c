#include "player.h"

#include <stdio.h>

void init_player(Player* player) {

    player->size = (Vector2){ screenWidth/10, 10 };
    player->position = (Vector2){ screenWidth/2, screenHeight-player->size.y};
    player->life = PLAYER_MAX_LIFE;
    player->score = 0;
    player->doubleRacket = false;
    player->halfRacket = false;
}

void updatePlayerScore(Player* player, const int points) {
    player->score += points;
    printf("Score: %d\n", player->score);
}

void add_life(Player* player) {
    if (player->life < PLAYER_MAX_LIFE) player->life++;
}

void double_racket(Player* player) {
    player->doubleRacket = true;
}

void half_racket(Player* player) {
    player->halfRacket = true;
}

void update_player_movement(Player *player, float screenWidth) {
    if (IsKeyDown(KEY_A)) player->position.x -= 5;
    if (IsKeyDown(KEY_D)) player->position.x += 5;

    // Limitar el movimiento del jugador a la pantalla
    if ((player->position.x - player->size.x / 2) <= 0) player->position.x = player->size.x / 2;
    if ((player->position.x + player->size.x / 2) >= screenWidth) player->position.x = screenWidth - player->size.x / 2;
}