#ifndef COM_SERVER_H
#define COM_SERVER_H

// Inicia el servidor de comunicación
void startComServer();

// Enviar mensajes desde main a JsonProcessor y luego a SocketServer
void sendMessageToServer(const char *message);

// Recibir mensajes ya procesados desde el servidor, listos para ser usados en el main
char *getProcessedMessage();

// Bucle que maneja la escucha de mensajes entrantes del servidor
_Noreturn void *messageListeningLoop(void *arg);

#endif // COM_SERVER_H
