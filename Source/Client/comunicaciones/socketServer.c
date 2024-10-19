#include "socketServer.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345

int sock = 0;
struct sockaddr_in serv_addr;
char buffer[1024] = {0};

/*
 * Inicia la conexión de socket con el servidor
 */
void startSocketConnection() {
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error al crear el socket\n");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("Dirección IP inválida\n");
        return;
    }

    while (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error al conectar al servidor, reintentando en 1 segundo...\n");
        sleep(1);
    }

    printf("Conectado al servidor\n");
}

/*
 * Enviar un mensaje JSON al servidor
 */
void sendToServer(const char *jsonMessage) {
    send(sock, jsonMessage, strlen(jsonMessage), 0);
    printf("Mensaje enviado al servidor: %s\n", jsonMessage);
}

/*
 * Recibir un mensaje del servidor
 */
int receiveFromServer(char *buffer) {
    int valread = read(sock, buffer, 1024);
    buffer[valread] = '\0';
    return valread;
}
