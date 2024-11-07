#include "game.h"
#include "comunicaciones/comServer.h"
#include <stdlib.h>
#include <time.h>

float lastUpdateTime = 0.0f; // Último tiempo de actualización
float deltaTime = 0.0f; // Tiempo transcurrido desde la última actualización
float ball_speed_multiplier = 1.0f;


float reconnectionTimer = 0.0f;
const float reconnectionInterval = 2.0f; // Intento de reconexión cada 2 segundo

// Inicializar variables de juego
void InitGame() {
    // Inicializar el jugador
    player.position = (Vector2) {screenWidth / 2, screenHeight * 7 / 8};
    player.size = (Vector2) {screenWidth / 10, 10};
    player.life = PLAYER_MAX_LIFE;
    player.score = 0;

    // Inicializar las bolas
    for (int i = 0; i < MAX_BALLS; i++) {
        balls[i].position = (Vector2) {screenWidth / 2, screenHeight * 7 / 8 - 30};
        balls[i].speed = (Vector2) {0, 0};
        balls[i].radius = 7;
        balls[i].active = false;
    }

    // Inicializar los ladrillos
    brickSize = (Vector2) {GetScreenWidth() / BRICKS_PER_LINE, 20};
    int initialDownPosition = 50;

    for (int i = 0; i < LINES_OF_BRICKS; i++) {
        for (int j = 0; j < BRICKS_PER_LINE; j++) {
            brick[i][j].position = (Vector2) {j * brickSize.x + brickSize.x / 2, i * brickSize.y + initialDownPosition};
            brick[i][j].active = true;
            brick[i][j].points = (i < 2) ? 400 : (i < 4) ? 300 : (i < 6) ? 200 : 100;
        }
    }
}
// Función para verificar colisión entre una bola y un ladrillo
bool CheckCollisionBallBrick(Ball ball, Brick brick) {
    // Revisamos solo si el ladrillo está activo
    if (!brick.active) {
        return false;
    }

    // Verificar la colisión usando la distancia de la bola a los bordes del ladrillo
    float deltaX = ball.position.x - fmaxf(brick.position.x - brickSize.x / 2, fminf(ball.position.x, brick.position.x + brickSize.x / 2));
    float deltaY = ball.position.y - fmaxf(brick.position.y - brickSize.y / 2, fminf(ball.position.y, brick.position.y + brickSize.y / 2));

    return (deltaX * deltaX + deltaY * deltaY) < (ball.radius * ball.radius);
}


// Función para manejar la colisión de una bola con un ladrillo
void check_brick(int i, int j) {
    if (brick[i][j].active) {
        applyPower(brick[i][j].power); // Aplica el poder del ladrillo
        brick[i][j].active = false;    // Desactiva el ladrillo
    }
}

// Función para aplicar los poderes
void applyPower(BrickPower power) {
    if (power.addLife) add_life();
    if (power.addBall) add_ball(player.position.x, player.position.y);
    if (power.doubleRacket) doubleRacket();
    if (power.halfRacket) halfRacket();
    if (power.speedUp) speedUp();
    if (power.speedDown) speedDown();
}

// Funciones de poderes específicos
void add_ball(int posX, int posY) {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (!balls[i].active) {
            balls[i].active = true;
            balls[i].position = (Vector2){ posX, posY };
            balls[i].speed = (Vector2){0, -5};
            break;
        }
    }
}

void add_life() {
    player.life++;
}

void doubleRacket() {
    player.size = (Vector2){ screenWidth / 5, 10 };
}

void halfRacket() {
    player.size = (Vector2){ screenWidth / 20, 10 };
}

void speedUp() {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (balls[i].active) {
            balls[i].speed.y *= 2;
            balls[i].speed.x *= 2;
        }
    }
}

void speedDown() {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (balls[i].active) {
            balls[i].speed.y /= 2;
            balls[i].speed.x /= 2;
        }
    }
}

// Función para actualizar el poder del ladrillo desde el servidor
void update_brick_power(int i, int j, BrickPower newPower) {
    if (i >= 0 && i < LINES_OF_BRICKS && j >= 0 && j < BRICKS_PER_LINE) {
        brick[i][j].power = newPower;
    }
}

void update_player_score(int brickx, int bricky) {
    player.score += brick[brickx][bricky].points;
}

void UpdateGame() {
    if (!isConnected) {
        // Incrementa el temporizador de reconexión
        reconnectionTimer += deltaTime;
        ComServer *comServer = ComServer_create("player");


        // Intenta reconectar al servidor en intervalos regulares
        if (reconnectionTimer >= reconnectionInterval) {
            reconnectionTimer = 0.0f;
            SocketServer_reconnect(comServer->socketServer);

            if (SocketServer_isConnected(comServer->socketServer)) {
                isConnected = true;
                printf("Reconectado al servidor\n");
                // Reinicia el juego si estaba en pausa por desconexión
                if (currentScreen == GAME || currentScreen == SPECTATOR) {
                    Pause = false;
                }
            } else {
                printf("Intentando reconectar...\n");
            }
        }
        return; // Salir de UpdateGame si no está conectado
    }
    if (!gameOver) {
        if (IsKeyPressed('P')) Pause = !Pause;

        if (!Pause) {
            // Movimiento del jugador
            if (IsKeyDown(KEY_A)) player.position.x -= 5;
            if ((player.position.x - player.size.x / 2) <= 0) player.position.x = player.size.x / 2;
            if (IsKeyDown(KEY_D)) player.position.x += 5;
            if ((player.position.x + player.size.x / 2) >= screenWidth)
                player.position.x = screenWidth - player.size.x / 2;

            // Lanzar la bola
            if (!balls[0].active) {
                if (IsKeyPressed(KEY_W)) {
                    balls[0].active = true;
                    balls[0].speed = (Vector2){0, -5 * ball_speed_multiplier};
                }
            }

            for (int i = 0; i < MAX_BALLS; i++) {
                // Movimiento de la bola
                if (balls[i].active) {
                    balls[i].position.x += balls[i].speed.x;
                    balls[i].position.y += balls[i].speed.y;
                } else {
                    balls[i].position = (Vector2){player.position.x, screenHeight * 7 / 8 - 30};
                }

                // Colisión: bola vs paredes
                if ((balls[i].position.x + balls[i].radius >= screenWidth) ||
                    (balls[i].position.x - balls[i].radius <= 0))
                    balls[i].speed.x *= -1;
                if (balls[i].position.y - balls[i].radius <= 0) balls[i].speed.y *= -1;
                if (balls[i].position.y + balls[i].radius >= screenHeight) {
                    balls[i].speed = (Vector2){0, 0};
                    balls[i].active = false;

                    // Revisar si alguna bola sigue activa
                    bool anyBallActive = false;
                    for (int j = 0; j < MAX_BALLS; j++) {
                        if (balls[j].active) {
                            anyBallActive = true;
                            break;
                        }
                    }

                    // Si no hay bolas activas, reducir la vida del jugador
                    if (!anyBallActive) {
                        player.life--;
                    }
                }

                // Colisión: bola vs jugador
                if (CheckCollisionCircleRec(balls[i].position, balls[i].radius,
                                            (Rectangle){player.position.x - player.size.x / 2,
                                                        player.position.y - player.size.y / 2, player.size.x,
                                                        player.size.y})) {
                    if (balls[i].speed.y > 0) {
                        balls[i].speed.y *= -1;
                        balls[i].speed.x = (balls[i].position.x - player.position.x) / (player.size.x / 2) * 5;
                    }
                }
            }

            // Colisión: bola vs ladrillos
            for (int i = 0; i < LINES_OF_BRICKS; i++) {
                for (int j = 0; j < BRICKS_PER_LINE; j++) {
                    if (brick[i][j].active) {
                        for (int b = 0; b < MAX_BALLS; b++) {
                            if (CheckCollisionBallBrick(balls[b], brick[i][j])) {
                                brick[i][j].active = false;
                                update_player_score(i, j);
                                check_brick(i, j);
                            }
                        }
                    }
                }
            }

            // Verificar niveles completados y aumentar velocidad
            for (int level = 0; level < LINES_OF_BRICKS / 2; level++) {
                bool allBricksDestroyedInLevel = true;

                for (int i = level * 2; i < (level + 1) * 2; i++) {
                    for (int j = 0; j < BRICKS_PER_LINE; j++) {
                        if (brick[i][j].active) {
                            allBricksDestroyedInLevel = false;
                            break;
                        }
                    }
                    if (!allBricksDestroyedInLevel) break;
                }

                if (allBricksDestroyedInLevel && !levelSpeedChanged[level]) {
                    ball_speed_multiplier *= 1.2f;
                    for (int i = 0; i < MAX_BALLS; i++) {
                        if (balls[i].active) {
                            balls[i].speed.y *= ball_speed_multiplier;
                            balls[i].speed.x *= ball_speed_multiplier;
                            levelsCompleted++;
                        }
                    }
                    levelSpeedChanged[level] = true;
                }
            }

            // Verificar fin del juego
            if (player.life <= 0) {
                gameOver = true;
            } else {
                gameOver = true;
                for (int i = 0; i < LINES_OF_BRICKS; i++) {
                    for (int j = 0; j < BRICKS_PER_LINE; j++) {
                        if (brick[i][j].active) gameOver = false;
                    }
                }
            }
        }
    } else if (IsKeyPressed(KEY_ENTER)) {
        InitGame();
        gameOver = false;
        ball_speed_multiplier = 1.0f;
    }

    // Captura el tiempo actual
    float currentTime = (float)clock() / CLOCKS_PER_SEC;
    deltaTime = currentTime - lastUpdateTime;

    // Enviar el estado del juego si ha pasado el tiempo adecuado
    if (deltaTime >= (1.0f / 60.0f)) {
        char *jsonString = generateGameStateJSON();
        ComServer *comServer = ComServer_create("player");

        if (SocketServer_isConnected(comServer->socketServer)) {
            ComServer_sendMessage(comServer, jsonString);
        }

        free(jsonString);  // Liberar memoria después de enviar
        lastUpdateTime = currentTime;  // Actualizar el tiempo de la última generación
    }
}

void DrawErrorScreen() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("Connection lost...", screenWidth / 2 - MeasureText("Connection lost...", 20) / 2, screenHeight / 2 - 40, 20, DARKGRAY);
    DrawText("Attempting to reconnect", screenWidth / 2 - MeasureText("Attempting to reconnect", 20) / 2, screenHeight / 2, 20, DARKGRAY);
    DrawText("Please wait...", screenWidth / 2 - MeasureText("Please wait...", 20) / 2, screenHeight / 2 + 40, 20, GRAY);

    EndDrawing();
}

// Generar estado del juego en formato JSON
char *generateGameStateJSON() {
    // Crear objeto raíz JSON para el estado del juego
    cJSON *gameState = cJSON_CreateObject();
    cJSON_AddStringToObject(gameState, "command", "sendGameState");

    // Información del jugador
    cJSON *playerJSON = cJSON_CreateObject();
    cJSON_AddNumberToObject(playerJSON, "positionX", player.position.x);
    cJSON_AddNumberToObject(playerJSON, "positionY", player.position.y);
    cJSON_AddNumberToObject(playerJSON, "sizeX", player.size.x);
    cJSON_AddNumberToObject(playerJSON, "sizeY", player.size.y);
    cJSON_AddNumberToObject(playerJSON, "life", player.life);
    cJSON_AddNumberToObject(playerJSON, "score", player.score);
    cJSON_AddItemToObject(gameState, "player", playerJSON);

    // Añadir bolas activas
    cJSON *ballsArray = cJSON_CreateArray();
    for (int i = 0; i < MAX_BALLS; i++) {
        cJSON *ballJSON = cJSON_CreateObject();
        cJSON_AddBoolToObject(ballJSON, "active", balls[i].active);
        cJSON_AddNumberToObject(ballJSON, "positionX", balls[i].position.x);
        cJSON_AddNumberToObject(ballJSON, "positionY", balls[i].position.y);
        cJSON_AddItemToArray(ballsArray, ballJSON);
    }
    cJSON_AddItemToObject(gameState, "balls", ballsArray);

    // Convertir el objeto JSON a cadena de caracteres
    char *jsonString = cJSON_PrintUnformatted(gameState);

    // Limpiar memoria de cJSON
    cJSON_Delete(gameState);

    return jsonString;  // Recuerda liberar la memoria después de usar la cadena
}


void DrawGame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (isReconnecting) {
        DrawText("Reconectando... Espere por favor", screenWidth / 2 - MeasureText("Reconectando... Espere por favor", 20) / 2, screenHeight / 2, 20, DARKGRAY);
    }
    if (currentScreen == RECONNECTING) {
        // Mostrar la pantalla de reconexión
        DrawText("Reconectando... Espere por favor", screenWidth / 2 - MeasureText("Reconectando... Espere por favor", 20) / 2, screenHeight / 2, 20, DARKGRAY);
    }
    if (!gameOver) {
        // Dibujar el jugador
        DrawRectangle(player.position.x - player.size.x / 2, player.position.y - player.size.y / 2, player.size.x, player.size.y, BLACK);

        // Dibujar la vida del jugador
        for (int i = 0; i < player.life; i++) {
            DrawRectangle(20 + 40 * i, screenHeight - 30, 35, 10, LIGHTGRAY);
        }

        // Mostrar mensaje de pausa
        if (Pause) {
            DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 10, 40, GRAY);
        }

        // Dibujar la primera bola en su posición inicial
        DrawCircleV(balls[0].position, balls[0].radius, MAROON);

        // Dibujar la puntuación del jugador
        DrawText(TextFormat("SCORE: %04i", player.score), 20, 10, 20, DARKGRAY);

        // Dibujar el nivel completado
        DrawText(TextFormat("LEVEL: %01i/%01i", levelsCompleted, LINES_OF_BRICKS / 2), screenWidth - 120, 10, 20, DARKGRAY);

        // Dibujar todas las bolas activas
        for (int i = 0; i < MAX_BALLS; i++) {
            if (balls[i].active) {
                DrawCircleV(balls[i].position, balls[i].radius, MAROON);
            }
        }

        // Espaciado entre ladrillos
        float brickSpacing = 5.0f;

        // Dibujar los ladrillos
        for (int i = 0; i < LINES_OF_BRICKS; i++) {
            for (int j = 0; j < BRICKS_PER_LINE; j++) {
                if (brick[i][j].active) {
                    Color brickColor;

                    // Asignar color según la línea del ladrillo
                    if (i < 2) {
                        brickColor = RED;
                        brick[i][j].color = RED;
                    } else if (i < 4) {
                        brickColor = ORANGE;
                        brick[i][j].color = ORANGE;
                    } else if (i < 6) {
                        brickColor = YELLOW;
                        brick[i][j].color = YELLOW;
                    } else {
                        brickColor = GREEN;
                        brick[i][j].color = GREEN;
                    }

                    // Dibuja el ladrillo con el color correspondiente
                    DrawRectangle(
                        brick[i][j].position.x - (brickSize.x / 2) + brickSpacing / 2,
                        brick[i][j].position.y - (brickSize.y / 2) + brickSpacing / 2,
                        brickSize.x - brickSpacing,
                        brickSize.y - brickSpacing,
                        brickColor
                    );
                }
            }
        }
    } else {
        // Mostrar mensaje de fin del juego
        DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);
    }

    EndDrawing();
}

// Liberar recursos del juego
void UnloadGame() {
    // Actualmente no hay recursos específicos para liberar,
    // pero esta función queda preparada para futuros cambios.
}

// Actualizar y dibujar cada fotograma
void UpdateDrawFrame(){
    if (isConnected) {
        UpdateGame(); // Solo actualiza el juego si está conectado
        DrawGame();
    } else {
        DrawErrorScreen(); // Muestra la pantalla de error si no está conectado
    }
}



void processUpdateBrickMessage(cJSON *data) {
    if (!data) return;

    cJSON *x = cJSON_GetObjectItem(data, "x");
    cJSON *y = cJSON_GetObjectItem(data, "y");

    if (x && y && cJSON_IsNumber(x) && cJSON_IsNumber(y)) {
        int brickX = x->valueint;
        int brickY = y->valueint;

        BrickPower power = {0};

        // Extraer los poderes del JSON
        cJSON *addLife = cJSON_GetObjectItem(data, "addLife");
        cJSON *addBall = cJSON_GetObjectItem(data, "addBall");
        cJSON *doubleRacket = cJSON_GetObjectItem(data, "doubleRacket");
        cJSON *halfRacket = cJSON_GetObjectItem(data, "halfRacket");
        cJSON *speedUp = cJSON_GetObjectItem(data, "speedUp");
        cJSON *speedDown = cJSON_GetObjectItem(data, "speedDown");

        // Asignar valores a la estructura de poder
        power.addLife = addLife && cJSON_IsBool(addLife) ? cJSON_IsTrue(addLife) : false;
        power.addBall = addBall && cJSON_IsBool(addBall) ? cJSON_IsTrue(addBall) : false;
        power.doubleRacket = doubleRacket && cJSON_IsBool(doubleRacket) ? cJSON_IsTrue(doubleRacket) : false;
        power.halfRacket = halfRacket && cJSON_IsBool(halfRacket) ? cJSON_IsTrue(halfRacket) : false;
        power.speedUp = speedUp && cJSON_IsBool(speedUp) ? cJSON_IsTrue(speedUp) : false;
        power.speedDown = speedDown && cJSON_IsBool(speedDown) ? cJSON_IsTrue(speedDown) : false;

        // Actualizar el ladrillo con los poderes
        update_brick_power(brickX, brickY, power);
    }
}