//
// Created by jimmy on 08/10/24.
//

#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "cjson/cJSON.h"

/*
 * Metodos para recibir comandos del servidor
 */
void setNumber(int n);

/*
 * Metodos para enviar y recibir mensajes del servidor
 */

void start_server();

void connectToServer();

int readServerMessage();

void processServerMessage();

void sendJsontoServer(const char *json);

void sendClientType(const char *type);

#endif //CLIENT_CONNECTION_H
