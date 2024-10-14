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
        } else {
            gameOver = true;
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
        // Draw player bar
        DrawRectangle(player.position.x - player.size.x/2, player.position.y - player.size.y/2, player.size.x, player.size.y, BLACK);

        // Draw player lives
        for (int i = 0; i < player.life; i++) DrawRectangle(20 + 40*i, screenHeight - 30, 35, 10, LIGHTGRAY);

        if (Pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, GRAY);

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

