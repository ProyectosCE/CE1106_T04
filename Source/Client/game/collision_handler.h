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
#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include "../game_status.h"

void handle_collisions(GameState *game_state);
bool bloquesEliminados(GameState *game_state);

#endif // COLLISION_HANDLER_H
