//
// Created by jimmy on 12/10/24.
//

#include "spectator.h"


void DrawSpectator() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("You are a spectator", screenWidth / 2 - MeasureText("You are a spectator", 20) / 2, screenHeight / 2, 20, DARKGRAY);
    DrawText("Press ESC to exit", screenWidth / 2 - MeasureText("Press ESC to exit", 20) / 2, screenHeight / 2 + 40, 20, GRAY);

    EndDrawing();
}