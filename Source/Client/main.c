#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "cjson/cJSON.h"

#define PORT 12345
#define SERVER_IP "127.0.0.1"

/*
 * Metodos para recibir comandos del servidor
 */

int number;

void setNumber(int n) {
    number = n;
    printf("Número actualizado: %d\n", number);
}

/*
 *
 * Metodos para comunicarse con el servidor
 *
 */

void connectToServer(int sock, struct sockaddr_in serv_addr) {
    // Convertir dirección IP a binario
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("Dirección IP inválida\n");
        return;
    }
    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error al conectar al servidor\n");
        return;
    }
    printf("Conectado al servidor\n");
}

void readServerMessage(int sock, char *buffer) {
    int valread = read(sock, buffer, 1024);
    buffer[valread] = '\0';  // Asegurarse de que la cadena esté terminada
    printf("Respuesta del servidor (JSON): %s\n", buffer);
}

void processServerMessage(const char *buffer) {
    // Parsear el JSON recibido
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        printf("Error al parsear el JSON del servidor\n");
        return;
    }

    // Obtener el valor de "command" del JSON
    cJSON *command = cJSON_GetObjectItemCaseSensitive(json, "command");
    if (cJSON_IsString(command) && (command->valuestring != NULL)) {
        printf("Comando recibido: %s\n", command->valuestring);

        // Si el comando es "setNumber", obtener el número y actualizarlo
        if (strcmp(command->valuestring, "setNumber") == 0) {
            cJSON *numberItem = cJSON_GetObjectItemCaseSensitive(json, "number");
            if (cJSON_IsNumber(numberItem)) {
                setNumber(numberItem->valueint);
            } else {
                printf("Error: número no válido en el JSON\n");
            }
        } else {
            printf("Comando desconocido: %s\n", command->valuestring);
        }
    } else if (cJSON_IsNull(command)) {
        printf("Error: comando no encontrado en el JSON\n");
    } else {
        printf("Error: comando no es una cadena\n");
    }

    // Limpiar memoria JSON
    cJSON_Delete(json);
}

void sendJsontoServer(int sock, const char *json) {
    send(sock, json, strlen(json), 0);
    printf("Mensaje enviado al servidor: %s\n", json);
}


int main(void) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Crear socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error al crear el socket\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    connectToServer(sock, serv_addr);
    readServerMessage(sock, buffer);
    processServerMessage(buffer);

    close(sock);
    return 0;
}

