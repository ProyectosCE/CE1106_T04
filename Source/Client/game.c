//
// Created by jimmy on 12/10/24.
//

#include "game.h"

// Initialize game variables
void InitGame(){
}

void UpdateGame(){
    if (!gameOver)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            gameOver = true;
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            gameOver = false;
        }
    }
}

void DrawGame(){
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (!gameOver)
    {
        DrawText("TEST", 190, 200, 20, LIGHTGRAY);
    }
    EndDrawing();
}

void UnloadGame(){
}

void UpdateDrawFrame(){
    UpdateGame();
    DrawGame();
}

