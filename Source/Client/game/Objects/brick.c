#include "brick.h"

void init_bricks(GameState* gameState, Vector2 brickSize) {
    // Inicializar los ladrillos

    int initialDownPosition = 50;

    int points;
    Color color;
    for (int i = 0; i < LINES_OF_BRICKS; i++) {
        // Determinar los puntos y color según la fila
        if (i < 2) {
            points = 400;
            color = RED;
        } else if (i < 4) {
            points = 300;
            color = ORANGE;
        } else if (i < 6) {
            points = 200;
            color = YELLOW;
        } else {
            points = 100;
            color = GREEN;
        }

        for (int j = 0; j < BRICKS_PER_LINE; j++) {
            // Inicializar cada ladrillo en la matriz
            gameState->bricks[i][j].position = (Vector2){
                j * brickSize.x + brickSize.x / 2,
                i * brickSize.y + initialDownPosition
            };
            gameState->bricks[i][j].active = true;
            gameState->bricks[i][j].points = points;
            gameState->bricks[i][j].color = color;  // Asigna el color correspondiente
        }
    }
}


void deactivate_brick(Brick* brick) {
    brick->active = false;
}


