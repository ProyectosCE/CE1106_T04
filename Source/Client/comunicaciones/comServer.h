#ifndef COM_SERVER_H
#define COM_SERVER_H

/*
 * Funciones para manejar la comunicación entre main, SocketServer y JsonProcessor.
 */
void startComServer(); // Inicia el servidor de comunicación
void sendMessageToServer(const char *message); // Enviar mensajes desde main a JsonProcessor y luego a SocketServer
void processIncomingMessage(const char *message); // Procesar mensajes entrantes desde SocketServer y devolverlos a main

#endif // COM_SERVER_H
