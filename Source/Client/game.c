//
// Created by jimmy on 12/10/24.
//

#include "game.h"

float lastUpdateTime = 0.0f; // Último tiempo de actualización.
float deltaTime = 0.0f; // Tiempo transcurrido desde la última actualización.
float ball_speed_multiplier = 1.0f;

// Initialize game variables
void InitGame(){
    // Inicializar el jugador
    player.position = (Vector2){ screenWidth/2, screenHeight*7/8 };
    player.size = (Vector2){ screenWidth/10, 10 };
    player.life = PLAYER_MAX_LIFE;
    player.score = 0;

    // Inicializar las bolas
    for (int i = 0; i < MAX_BALLS; i++) {
        balls[i].position = (Vector2){ screenWidth/2, screenHeight*7/8 - 30 };
        balls[i].speed = (Vector2){ 0, 0 };
        balls[i].radius = 7;
        balls[i].active = false;
    }

    // Inicializar los ladrillos
    brickSize = (Vector2){ GetScreenWidth()/BRICKS_PER_LINE, 20 };

    int initialDownPosition = 50;

    for (int i = 0; i < LINES_OF_BRICKS; i++)
    {
        for (int j = 0; j < BRICKS_PER_LINE; j++)
        {
            brick[i][j].position = (Vector2){ j*brickSize.x + brickSize.x/2, i*brickSize.y + initialDownPosition };
            brick[i][j].active = true;
        }
    }

    // Inicializar los puntos de los ladrillos
    for (int i = 0; i < LINES_OF_BRICKS; i++) {
        for (int j = 0; j < BRICKS_PER_LINE; j++) {
            if (brick[i][j].active) {
                if (i < 2) {
                    brick[i][j].points = 400;
                } else if (i < 4) {
                    brick[i][j].points = 300;
                } else if (i < 6) {
                    brick[i][j].points = 200;
                } else {
                    brick[i][j].points = 100;
                }
            }
        }
    }
}

void UpdateGame(){
    if (!gameOver) {

        if (IsKeyPressed('P')) Pause = !Pause;

        if (!Pause) {
            // Logica del juego
            if (IsKeyDown(KEY_LEFT)) player.position.x -= 5;
            if ((player.position.x - player.size.x / 2) <= 0) player.position.x = player.size.x / 2;
            if (IsKeyDown(KEY_RIGHT)) player.position.x += 5;
            if ((player.position.x + player.size.x / 2) >= screenWidth)
                player.position.x = screenWidth - player.size.x / 2;

            // Lanzar la bola
            if (!balls[0].active) {
                if (IsKeyPressed(KEY_SPACE)) {
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
                    balls[i].position = (Vector2) {player.position.x, screenHeight * 7 / 8 - 30};
                }

                // Logica de Colision: bola vs paredes
                if (((balls[i].position.x + balls[i].radius) >= screenWidth) ||
                    ((balls[i].position.x - balls[i].radius) <= 0))
                    balls[i].speed.x *= -1;
                if ((balls[i].position.y - balls[i].radius) <= 0) balls[i].speed.y *= -1;
                if ((balls[i].position.y + balls[i].radius) >= screenHeight) {
                    balls[i].speed = (Vector2) {0, 0};

                    balls[i].active = false;

                    // Comprobar si queda alguna bola activa
                    bool anyBallActive = false;
                    for (int j = 0; j < MAX_BALLS; j++) {
                        if (balls[j].active) {
                            anyBallActive = true;
                            break;
                        }
                    }

                    // Si no hay más bolas activas, reducir la vida del jugador
                    if (!anyBallActive) {
                        player.life--;
                    }
                }


                // Logica de Colision: bola vs jugador
                if (CheckCollisionCircleRec(balls[i].position, balls[i].radius,
                                            (Rectangle) {player.position.x - player.size.x / 2,
                                                         player.position.y - player.size.y / 2, player.size.x,
                                                         player.size.y})) {
                    if (balls[i].speed.y > 0) {
                        balls[i].speed.y *= -1;
                        balls[i].speed.x = (balls[i].position.x - player.position.x) / (player.size.x / 2) * 5;
                    }
                }
            }

            // Collision logic: ball vs bricks
            for (int i = 0; i < LINES_OF_BRICKS; i++) {
                for (int j = 0; j < BRICKS_PER_LINE; j++) {
                    if (brick[i][j].active) {
                        for (int b = 0; b < MAX_BALLS; b++) {
                            // Hit below
                            if (((balls[b].position.y - balls[b].radius) <=
                                 (brick[i][j].position.y + brickSize.y / 2)) &&
                                ((balls[b].position.y - balls[b].radius) >
                                 (brick[i][j].position.y + brickSize.y / 2 + balls[b].speed.y)) &&
                                ((fabs(balls[b].position.x - brick[i][j].position.x)) <
                                 (brickSize.x / 2 + balls[b].radius * 2 / 3)) && (balls[b].speed.y < 0)) {
                                brick[i][j].active = false;
                                balls[b].speed.y *= -1;

                                update_player_score(i, j);
                                check_brick(i, j);


                            }
                                // Hit above
                            else if (((balls[b].position.y + balls[b].radius) >=
                                      (brick[i][j].position.y - brickSize.y / 2)) &&
                                     ((balls[b].position.y + balls[b].radius) <
                                      (brick[i][j].position.y - brickSize.y / 2 + balls[b].speed.y)) &&
                                     ((fabs(balls[b].position.x - brick[i][j].position.x)) <
                                      (brickSize.x / 2 + balls[b].radius * 2 / 3)) && (balls[b].speed.y > 0)) {
                                brick[i][j].active = false;
                                balls[b].speed.y *= -1;

                                update_player_score(i, j);
                                check_brick(i, j);
                            }
                                // Hit left
                            else if (((balls[b].position.x + balls[b].radius) >=
                                      (brick[i][j].position.x - brickSize.x / 2)) &&
                                     ((balls[b].position.x + balls[b].radius) <
                                      (brick[i][j].position.x - brickSize.x / 2 + balls[b].speed.x)) &&
                                     ((fabs(balls[b].position.y - brick[i][j].position.y)) <
                                      (brickSize.y / 2 + balls[b].radius * 2 / 3)) && (balls[b].speed.x > 0)) {
                                brick[i][j].active = false;
                                balls[b].speed.x *= -1;

                                update_player_score(i, j);
                                check_brick(i, j);
                            }
                                // Hit right
                            else if (((balls[b].position.x - balls[b].radius) <=
                                      (brick[i][j].position.x + brickSize.x / 2)) &&
                                     ((balls[b].position.x - balls[b].radius) >
                                      (brick[i][j].position.x + brickSize.x / 2 + balls[b].speed.x)) &&
                                     ((fabs(balls[b].position.y - brick[i][j].position.y)) <
                                      (brickSize.y / 2 + balls[b].radius * 2 / 3)) && (balls[b].speed.x < 0)) {
                                brick[i][j].active = false;
                                balls[b].speed.x *= -1;

                                update_player_score(i, j);
                                check_brick(i, j);
                            }
                        }
                    }
                }
            }


            // Comprobar si los ladrillos de cada nivel específico fueron destruidos
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

                // Si los ladrillos de este nivel fueron destruidos y la velocidad no ha cambiado aún
                if (allBricksDestroyedInLevel && !levelSpeedChanged[level]) {
                    ball_speed_multiplier *= 1.2f;
                    for (int i = 0; i < MAX_BALLS; i++) {
                        if (balls[i].active) {
                            // Incrementa la velocidad de la bola en un 30%
                            balls[i].speed.y *= ball_speed_multiplier;
                            balls[i].speed.x *= ball_speed_multiplier;
                            levelsCompleted++;
                        }
                    }
                    levelSpeedChanged[level] = true; // Marca que la velocidad ya cambió para este nivel
                }
            }

            if (player.life <= 0) gameOver = true;
            else
            {
                gameOver = true;

                for (int i = 0; i < LINES_OF_BRICKS; i++)
                {
                    for (int j = 0; j < BRICKS_PER_LINE; j++)
                    {
                        if (brick[i][j].active) gameOver = false;
                    }
                }
            }
        }
    }
    else {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            gameOver = false;
            ball_speed_multiplier = 1.0f;
        }
    }

    // Enviar JSON
}

void DrawGame(){
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (!gameOver)
    {
        // Dibujar el jugador
        DrawRectangle(player.position.x - player.size.x/2, player.position.y - player.size.y/2, player.size.x, player.size.y, BLACK);

        // Dibujar la vida del jugador
        for (int i = 0; i < player.life; i++) DrawRectangle(20 + 40*i, screenHeight - 30, 35, 10, LIGHTGRAY);

        if (Pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 10, 40, GRAY);

        // Dibujar la bola en la posición inicial
        DrawCircleV(balls[0].position, balls[0].radius, MAROON);

        // Dibujar la puntuación
        DrawText(TextFormat("SCORE: %04i", player.score), 20, 10, 20, DARKGRAY);

        // Dibujar niveles completados
        DrawText(TextFormat("LEVEL: %01i/%01i", levelsCompleted, LINES_OF_BRICKS/2), screenWidth - 120, 10, 20, DARKGRAY);


        // Dibujar todas las bolas activas
        for (int i = 0; i < MAX_BALLS; i++) {
            if (balls[i].active) {
                DrawCircleV(balls[i].position, balls[i].radius, MAROON);  // Dibuja la bola en su posición
            }
        }

        float brickSpacing = 5.0f;

        // Dibujar los ladrillos
        for (int i = 0; i < LINES_OF_BRICKS; i++)
        {
            for (int j = 0; j < BRICKS_PER_LINE; j++)
            {
                if (brick[i][j].active)
                {
                    Color brickColor;

                    if (i < 2) {
                        brickColor = RED;  // Nivel 4 (líneas 0 y 1)
                        brick[i][j].color = RED;
                    } else if (i < 4) {
                        brickColor = ORANGE; // Nivel 3 (líneas 2 y 3)
                        brick[i][j].color = ORANGE;
                    } else if (i < 6) {
                        brickColor = YELLOW; // Nivel 2 (líneas 4 y 5)
                        brick[i][j].color = YELLOW;
                    } else {
                        brickColor = GREEN;  // Nivel 1 (líneas 6 y 7)
                        brick[i][j].color = GREEN;
                    }

                    // Dibuja el ladrillo con el color correspondiente
                    DrawRectangle(brick[i][j].position.x - (brickSize.x / 2) + brickSpacing / 2,
                                  brick[i][j].position.y - (brickSize.y / 2) + brickSpacing / 2,
                                  brickSize.x - brickSpacing,
                                  brickSize.y - brickSpacing,
                                  brickColor);
                }
            }
        }
    } else {
        DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 - 50, 20, GRAY);
    }
    EndDrawing();
}

void UnloadGame(){
}

void UpdateDrawFrame(){
    UpdateGame();
    DrawGame();
}

void check_brick(int i, int j){
    if (brick[i][j].add_ball) {
        add_ball(brick[i][j].position.x, brick[i][j].position.y);
    } else if (brick[i][j].add_life) {
        add_life();
    } else if (brick[i][j].add_doubleRacket) {
        doubleRacket();
    } else if (brick[i][j].add_halfRacket) {
        halfRacket();
    } else if (brick[i][j].speedUp) {
        speedUp();
    } else if (brick[i][j].speedDown) {
        speedDown();
    }
}

void update_brick_score(int level, int new_points) {
    for (int i = 0; i < LINES_OF_BRICKS; i++) {
        for (int j = 0; j < BRICKS_PER_LINE; j++) {
            if (brick[i][j].active) {
                // Actualizar puntos únicamente en el nivel especificado
                if ((level == 4 && i < 2) ||
                    (level == 3 && i >= 2 && i < 4) ||
                    (level == 2 && i >= 4 && i < 6) ||
                    (level == 1 && i >= 6)) {

                    brick[i][j].points = new_points;
                }
            }
        }
    }
}



void add_ball(int posX, int posY) {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (!balls[i].active) {
            balls[i].active = true;
            balls[i].position = (Vector2){ posX, posY };  // Posición del ladrillo destruido
            balls[i].speed = (Vector2){0, -5};
            break;  // Salir del bucle una vez que activamos una nueva bola
        }
    }
}

void add_life(){
    player.life++;
}
void doubleRacket(){
    player.size = (Vector2){ screenWidth/5, 10 };
}
void halfRacket(){
    player.size = (Vector2){ screenWidth/20, 10 };
}
void speedUp(){
    for (int i = 0; i < MAX_BALLS; i++) {
        if (balls[i].active) {
            balls[i].speed.y *= 2;
            balls[i].speed.x *= 2;
        }
    }
}
void speedDown(){
    for (int i = 0; i < MAX_BALLS; i++) {
        if (balls[i].active) {
            balls[i].speed.y /= 2;
            balls[i].speed.x /= 2;
        }
    }
}


void update_brick_ball(int i, int j){
    brick[i][j].add_ball = true;
}

void update_brick_life(int i, int j){
    brick[i][j].add_life = true;
}

void update_brick_doubleRacket(int i, int j){
    brick[i][j].add_doubleRacket = true;
}

void update_brick_halfRacket(int i, int j){
    brick[i][j].add_halfRacket = true;
}

void update_brick_speedUp(int i, int j){
    brick[i][j].speedUp = true;
}

void update_brick_speedDown(int i, int j){
    brick[i][j].speedDown = true;
}

void update_player_score(int brickx, int bricky) {
    player.score += brick[brickx][bricky].points;
}