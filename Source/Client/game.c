//
// Created by jimmy on 12/10/24.
//

#include "game.h"

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
}

void UpdateGame(){
    if (!gameOver) {

        if (IsKeyPressed('P')) Pause = !Pause;

        if (!Pause) {
            // Logica del juego
            if (IsKeyDown(KEY_LEFT)) player.position.x -= 5;
            if ((player.position.x - player.size.x/2) <= 0) player.position.x = player.size.x/2;
            if (IsKeyDown(KEY_RIGHT)) player.position.x += 5;
            if ((player.position.x + player.size.x/2) >= screenWidth) player.position.x = screenWidth - player.size.x/2;

            // Lanzar la bola
            if (!balls[0].active)
            {
                if (IsKeyPressed(KEY_SPACE))
                {
                    balls[0].active = true;
                    balls[0].speed = (Vector2) {0, -5};
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

            // Logica de Game Over
            if (player.life <= 0) {
                gameOver = true;
            }
        }

    } else {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            gameOver = false;
        }
    }
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

        if (Pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, GRAY);

        // Dibujar la bola en la posición inicial
        DrawCircleV(balls[0].position, balls[0].radius, MAROON);

        // Dibujar todas las bolas activas
        for (int i = 0; i < MAX_BALLS; i++) {
            if (balls[i].active) {
                DrawCircleV(balls[i].position, balls[i].radius, MAROON);  // Dibuja la bola en su posición
            }
        }

        float brickSpacing = 5.0f;
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

