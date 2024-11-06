#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <stdbool.h>
#include "game_logic.h"
#include "game_screen.h"


static Vector2 brickSize = { 0 };

void init_game_server();
void unload_game_server();
void start_game();
void GameServer_handleMessage(const char *message);  // Manejar mensajes

#endif // GAME_SERVER_H