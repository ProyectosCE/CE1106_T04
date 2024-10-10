//
// Created by jimmy on 08/10/24.
//

#include "connection.h"

#define SERVER_IP "127.0.0.1"
#define PORT 12345

int sock = 0;
struct sockaddr_in serv_addr;
char buffer[1024] = {0};


/*
 * Metodos para recibir comandos del servidor
 */

int number;

void setNumber(int n) {
    number = n;
    printf("Número actualizado: %d\n", number);
}


/*
 * Metodos para enviar y recibir mensajes del servidor
 */

void start_server() {
    // Crear socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error al crear el socket\n");
        return;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    connectToServer();
    readServerMessage();
}

void connectToServer() {
    // Convertir dirección IP a binario
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("Dirección IP inválida\n");
        return;
    }
    // Reintentar hasta que se conecte
    while (1) {
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            printf("Error al conectar al servidor, reintentando en %d segundos...\n", 1);
            sleep(1);
        } else {
            printf("Conectado al servidor\n");
            break;
        }
    }
}

int readServerMessage() {
    int valread = read(sock, buffer, 1024);
    buffer[valread] = '\0';
    //printf("Respuesta del servidor (JSON): %s\n", buffer);
    processServerMessage();
    return 0;
}

void processServerMessage() {
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

void sendJsontoServer(const char *json) {
    send(sock, json, strlen(json), 0);
    printf("Mensaje enviado al servidor: %s\n", json);
}

void sendClientType(const char *type){
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "command", "connect");
    cJSON_AddStringToObject(json, "role", type);
    char *jsonString = cJSON_Print(json);
    sendJsontoServer(jsonString);

    free(jsonString);
    cJSON_Delete(json);
}
