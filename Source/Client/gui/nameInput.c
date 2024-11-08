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
#include "nameInput.h"

// BIBLIOTECAS EXTERNAS
#include <raylib.h>


/* Function: DrawNameInput
Descripción:
Renderiza la pantalla de entrada de nombre, mostrando un cuadro de texto para que el jugador ingrese su nombre
y un mensaje para proceder al siguiente paso al presionar Enter.

Params:
gameState - Puntero a la estructura `GameState` que contiene el nombre ingresado por el jugador.

Returns:
- void: No retorna valores.

Restriction:
- `gameState` debe ser un puntero válido con `playerName` correctamente inicializado.
- La biblioteca de gráficos debe estar inicializada antes de llamar a esta función.

Example:
DrawNameInput(gameState);
// Dibuja la pantalla de entrada de nombre.

Problems:
- Problema: Si `gameState` o `playerName` son nulos, no se renderizará correctamente.
- Solución: Validar los parámetros antes de proceder.
- Problema: Si el jugador ingresa un nombre que excede el ancho del texto visible, el texto podría truncarse.
- Solución: Implementar desplazamiento o truncamiento visual del texto.

References:
- Ninguna referencia externa específica.
*/

void DrawNameInput(GameState *gameState) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Enter your name:", 100, 100, 20, DARKGRAY);
    DrawText(gameState->playerName, 100, 130, 20, BLACK);
    DrawText("Press Enter to Play", 100, 160, 20, DARKGRAY);
    EndDrawing();
}
