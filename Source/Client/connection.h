//
// Created by jimmy on 08/10/24.
//

#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

/*
 * Metodos para recibir comandos del servidor
 */
void setNumber(int n);

/*
 * Metodos para enviar y recibir mensajes del servidor
 */

void start_server();

void connectToServer();

void readServerMessage();

void processServerMessage();

void sendJsontoServer(const char *json);

#endif //CLIENT_CONNECTION_H
