#include "brick.h"

void init_bricks(GameState* gameState, Vector2 brickSize) {
    // Ajuste para colocar ladrillos debajo del texto de puntaje

    int initialDownPosition;
    if (gameState->linesOfBricks == 1)
    { initialDownPosition = 150;}
    else if (gameState->linesOfBricks == 2) {
        initialDownPosition = 100; // Mayor margen para 1-2 líneas de ladrillos
    } else if (gameState->linesOfBricks <= 6) {
        initialDownPosition = 75; // Margen estándar para 3-6 líneas
    }
    else {
        initialDownPosition = 50; // Menor margen para 7+ líneas
    }

    int points;
    Color color;

    for (int i = 0; i < gameState->linesOfBricks; i++) {
        // Determinar los puntos y colores según la fila
        if (i < 2) { // Nivel 1
            points = 400;
            color = RED;
        } else if (i < 4) { // Nivel 2
            points = 300;
            color = ORANGE;
        } else if (i < 6) { // Nivel 3
            points = 200;
            color = YELLOW;
        } else if (i < 8) { // Nivel 4
            points = 150;
            color = GREEN;
        } else if (i < 10) { // Nivel 5
            points = 120;
            color = BLUE;
        } else if (i < 12) { // Nivel 6
            points = 100;
            color = PURPLE;
        } else if (i < 14) { // Nivel 7
            points = 80;
            color = SKYBLUE;
        } else if (i < 16) { // Nivel 8
            points = 60;
            color = PINK;
        } else if (i < 18) { // Nivel 9
            points = 40;
            color = DARKGRAY;
        } else { // Nivel 10
            points = 20;
            color = BEIGE;
        }

        for (int j = 0; j < gameState->bricksPerLine; j++) {
            // Inicializar cada ladrillo en la matriz
            gameState->bricks[i][j].position = (Vector2){
                j * brickSize.x + brickSize.x / 2,
                i * brickSize.y + initialDownPosition
            };
            gameState->bricks[i][j].active = true;
            gameState->bricks[i][j].power=NONE;
            gameState->bricks[i][j].points = points;
            gameState->bricks[i][j].color = color; // Asignar el color correspondiente
        }
    }
}


void deactivate_brick(Brick* brick) {
    brick->active = false;
}


