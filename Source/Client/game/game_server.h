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
#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <stdbool.h>

#include "game_screen.h"


static Vector2 brickSize = { 0 };

void init_game_server();
void unload_game_server();
void start_game();
void GameServer_handleMessage(const char *message);  // Manejar mensajes

// Prototipos de funciones para los hilos de juego

/**
 * Función que actualiza el estado del juego en un hilo separado.
 * @param arg Un puntero al GameState que contiene el estado del juego.
 * @return NULL al completar la ejecución.
 */
void *update_game_thread(void *arg);

/**
 * Función que dibuja el estado del juego en un hilo separado.
 * @param arg Un puntero al GameState que contiene el estado del juego.
 * @return NULL al completar la ejecución.
 */
void *draw_game_thread(void *arg);

/**
 * Función que envía el estado del juego a través de la red en un hilo separado.
 * @param arg Un puntero al GameState que contiene el estado del juego.
 * @return NULL al completar la ejecución.
 */
void *send_game_state_thread(void *arg);

#endif // GAME_SERVER_H