/*
================================== LICENCIA ==================================================
MIT License
Copyright (c) 2024 José Bernardo Barquero Bonilla,
                   Jose Eduardo Campos Salazar,
                   Jimmy Feng Feng,
                   Alexander Montero Vargas
Consulta el archivo LICENSE para más detalles.
==============================================================================================
*/

// BIBLIOTECAS DE PROYECTO
#include "camera.h"
#include "../game_status.h"
#include "main_menu.h"

// BIBLIOTECAS EXTERNAS
#include "raylib.h"



/* Function: UpdateMenu
   Descripción:
     Gestiona la lógica del menú principal. Permite alternar entre los modos de juego (jugador o espectador),
     habilitar/deshabilitar la cámara y proceder a la siguiente pantalla al presionar Enter.

   Params:
     (Ninguno)

   Returns:
     - void: No retorna valores.

   Restriction:
     - `setPlayer`, `isPlayer`, `setCameraEnabled`, `isCameraEnabled`, y `setCurrentScreen` deben estar correctamente implementados.
     - La función `start_camera` debe manejar errores de inicialización de cámara.

   Example:
     UpdateMenu();
     // Procesa la lógica del menú principal.

   Problems:
     - Problema: Si las teclas no están configuradas correctamente, las opciones del menú no responderán.
       - Solución: Asegurarse de que `IsKeyPressed` funcione con las teclas esperadas.
     - Problema: Si `start_camera` falla, la cámara no se iniciará correctamente.
       - Solución: Registrar errores en `start_camera` y manejar adecuadamente los fallos.

   References:
     - Ninguna referencia externa específica.
*/
void UpdateMenu() {
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
            setCurrentScreen(NAME_INPUT);
            if (isCameraEnabled()) {
                start_camera();
            } else {
            }
        } else {
            setCurrentScreen(OBSERVER_SELECT);
        }
    }
}


/* Function: DrawMenu
Descripción:
Renderiza el menú principal en pantalla, mostrando las opciones disponibles y resaltando
la selección actual.

Params:
(Ninguno)

Returns:
- void: No retorna valores.

Restriction:
- Las funciones `GetScreenWidth`, `GetScreenHeight`, `isPlayer`, y `isCameraEnabled` deben estar implementadas.
- La biblioteca de gráficos debe estar inicializada antes de llamar a esta función.

Example:
DrawMenu();
// Dibuja el menú principal en pantalla.

Problems:
- Problema: Si la biblioteca gráfica no está inicializada, puede causar un fallo.
- Solución: Asegurarse de inicializar la biblioteca gráfica antes de esta función.
- Problema: Si las funciones de medición de texto (`MeasureText`) no retornan valores precisos, el texto puede
desalinearse.
- Solución: Ajustar manualmente las posiciones si es necesario.

References:
- Ninguna referencia externa específica.
*/
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
