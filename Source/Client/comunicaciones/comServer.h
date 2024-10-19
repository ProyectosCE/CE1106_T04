#ifndef COM_SERVER_H
#define COM_SERVER_H

// Inicia el servidor de comunicación
void startComServer();

// Enviar mensajes desde main a JsonProcessor y luego a SocketServer
void sendMessageToServer(const char *message);

// Recibir mensajes desde el servidor ya procesados, listos para ser usados en el main
char *getProcessedMessage();

#endif // COM_SERVER_H
