//
// Created by amv22 on 16/11/2024.
//

#include "nameInput.h"

#include <raylib.h>


void DrawNameInput(GameState *gameState) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Enter your name:", 100, 100, 20, DARKGRAY);
    DrawText(gameState->playerName, 100, 130, 20, BLACK);
    DrawText("Press Enter to Play", 100, 160, 20, DARKGRAY);
    EndDrawing();
}
