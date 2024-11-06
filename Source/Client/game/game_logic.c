#include "game_logic.h"
#include "raylib.h"
#include "Objects/player.h"
#include "Objects/ball.h"


#include "collision_handler.h"

void update_game(GameState *gameState) {
    if (IsKeyPressed(KEY_P)) gameState->pause = !gameState->pause;

    if (!gameState->pause) {

        update_player_movement(&gameState->player, screenWidth);
        update_ball_positions(&gameState->player, gameState->balls);
        handle_collisions(gameState);

        // Verificar si el jugador pierde todas las vidas
        if (gameState->player.life <= 0) {
            gameState->gameOver = true;  // Cambiar al menú principal en caso de Game Over
        }
    }
    if (gameState->gameOver) {
        if (IsKeyPressed(KEY_ENTER)) gameState->restart=true;
    }

}


