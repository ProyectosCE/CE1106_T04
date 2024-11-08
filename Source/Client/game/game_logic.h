/*
================================== LICENCIA ==================================================
MIT License
Copyright (c) 2024 José Bernardo Barquero Bonilla,
                   Jose Eduardo Campos Salazar,
                   Jimmy Feng Feng,
                   Alexander Montero Vargas
Consulta el archivo LICENSE para más detalles.
==============================================================================================
*/
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "../game_status.h"


extern pthread_t sendStateThread;
void *send_game_state_thread(void *arg);
void update_game(GameState* gameState);
void sendGameState(GameState *gameState);
void process_brick_update(const char* json_command);

#endif // GAME_LOGIC_H
