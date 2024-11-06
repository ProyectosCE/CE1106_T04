#include <stdio.h>
#include <malloc.h>
#include "main_menu.h"
#include "raylib.h"
#include "camera.h"
#include "../game_status.h"

void UpdateMenu() {
    ComServer *comServer = ComServer_create();
    // Cambia el modo de juego (jugador/espectador)
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) {
        setPlayer(!isPlayer());
    }

    // Activa o desactiva la cámara
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT)) {
        setCameraEnabled(!isCameraEnabled());
    }

    // Acción al presionar Enter
    if (IsKeyPressed(KEY_ENTER)) {
        if (isPlayer()) {
            // Inicializar el servidor de juego y el juego en sí
            init_game_server();
            setCurrentScreen(GAME);
            ComServer_sendMessage(comServer, "player");

            if (isCameraEnabled()) {
                ComServer_sendMessage(comServer, "camera_enabled");
                start_camera();
            } else {
                ComServer_sendMessage(comServer, "camera_disabled");
            }
        } else {
            setCurrentScreen(SPECTATOR);
            ComServer_sendMessage(comServer, "spectator");
        }
    }
}

void DrawMenu() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("Choose Mode:", screenWidth / 2 - MeasureText("Choose Mode:", 20) / 2, screenHeight / 2 - 60, 20, DARKGRAY);
    DrawText("1. Player", screenWidth / 2 - MeasureText("1. Player", 20) / 2, screenHeight / 2 - 20, 20, isPlayer() ? BLACK : GRAY);
    DrawText("2. Spectator", screenWidth / 2 - MeasureText("2. Spectator", 20) / 2, screenHeight / 2 + 20, 20, !isPlayer() ? BLACK : GRAY);
    DrawText("Game Camera: ", screenWidth / 2 - MeasureText("Game Camera: ", 20) / 2, screenHeight / 2 + 60, 20, DARKGRAY);
    DrawText(isCameraEnabled() ? "Enabled" : "Disabled", screenWidth / 2 + MeasureText("Game Camera: ", 20) / 2, screenHeight / 2 + 60, 20, isCameraEnabled() ? GREEN : RED);
    DrawText("Press Enter to Play", screenWidth / 2 - MeasureText("Press Enter to Play", 20) / 2, screenHeight / 2 + 120, 20, DARKGRAY);

    EndDrawing();
}
