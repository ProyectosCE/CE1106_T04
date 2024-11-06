#include "game_screen.h"
#include "raylib.h"

void draw_game(const GameState *gameState) {
    BeginDrawing();
    ClearBackground(RAYWHITE);


    if (!gameState->gameOver) {
        if (!gameState->pause){


        // Dibujar el jugador (raqueta)
        DrawRectangle(gameState->player.position.x - gameState->player.size.x/2,
                      gameState->player.position.y - gameState->player.size.y/2,
                      gameState->player.size.x,
                      gameState->player.size.x, BLUE);

        // Dibujar las vidas del jugador en la parte inferior de la pantalla
        for (int i = 0; i < gameState->player.life; i++) {
            DrawRectangle(20 + 40 * i, GetScreenHeight() - 30, 35, 10, LIGHTGRAY);
        }

        // Dibujar la puntuación
        DrawText(TextFormat("SCORE: %04i", gameState->player.score), 20, 10, 20, DARKGRAY);

        // Dibujar los niveles completados
        DrawText(TextFormat("LEVEL: %01i", gameState->levelsCompleted), GetScreenWidth() - 120, 10, 20, DARKGRAY);

        if (gameState->pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2,screenHeight/2 -10,40,RED);

        //Dibujar bola inicial
        DrawCircleV(gameState->balls[0].position, gameState->balls[0].radius, MAROON);

        // Dibujar todas las bolas activas desde el arreglo dinámico
        for (int i = 0; i < MAX_BALLS; i++) {
            if (gameState->balls[i].active) {
                DrawCircleV(gameState->balls[i].position, gameState->balls[i].radius, MAROON);
            }
        }

        // Dibujar los ladrillos activos con color según la línea
        for (int i = 0; i < LINES_OF_BRICKS; i++) {
            for (int j = 0; j < BRICKS_PER_LINE; j++) {
                // Verificar si el ladrillo está activo
                if (gameState->bricks[i][j].active) {
                    // Calcular las posiciones y tamaños de los ladrillos
                    float x = gameState->bricks[i][j].position.x - (gameState->brickSize.x / 2) + brickSpacing / 2;
                    float y = gameState->bricks[i][j].position.y - (gameState->brickSize.y / 2) + brickSpacing / 2;
                    float width = gameState->brickSize.x - brickSpacing;
                    float height = gameState->brickSize.y - brickSpacing;

                    // Dibujar el ladrillo
                    DrawRectangle(x, y, width, height, gameState->bricks[i][j].color);
                }
            }
        }
        }
        else {
            DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 10, 40, GRAY);
        }
    } else {
        // Pantalla de "Game Over"
        DrawText("PRESS [ENTER] TO PLAY AGAIN", screenWidth / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, screenHeight / 2 - 50, 20, GRAY);
    }

    EndDrawing();
}
