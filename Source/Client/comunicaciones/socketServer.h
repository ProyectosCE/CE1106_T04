#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

/*
 * Funciones para manejar la conexión de socket
 */
void startSocketConnection(); // Inicia la conexión del socket con el servidor
void sendToServer(const char *jsonMessage); // Envía un mensaje JSON al servidor
int receiveFromServer(char *buffer); // Recibe un mensaje del servidor y lo almacena en el buffer

#endif // SOCKET_SERVER_H
