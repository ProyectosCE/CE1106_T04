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
 * Recibir un mensaje del servidor. Se maneja la lectura iterativa
 * para asegurarse de que el mensaje completo es recibido.
 */
int receiveFromServer(char *buffer) {
    int totalBytesRead = 0;
    int bytesRead = 0;

    // Asegurar que el buffer esté vacío
    memset(buffer, 0, 1024);

    // Leer desde el socket mientras haya datos
    while ((bytesRead = read(sock, buffer + totalBytesRead, 1024 - totalBytesRead)) > 0) {
        totalBytesRead += bytesRead;

        // Si ya se ha leído todo el mensaje
        if (totalBytesRead >= 1024 || buffer[totalBytesRead - 1] == '\0') {
            break;
        }
    }

    if (totalBytesRead == 0) {
        printf("Conexión cerrada por el servidor\n");
        return -1;  // Error al recibir mensaje
    } else if (bytesRead < 0) {
        printf("Error al recibir el mensaje del servidor\n");
        return -1;  // Error en la lectura
    }

    return totalBytesRead;
}
