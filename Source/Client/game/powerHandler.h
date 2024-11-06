#ifndef POWERHANDLER_H
#define POWERHANDLER_H


void initPowerHandler();
// Funciones de manejo de ladrillos y puntuación
void check_brick(int brickx, int bricky);
void update_brick_score(int level, int new_points);

// Funciones de manejo de bolas
void add_ball(int posX, int posY);

// Funciones de manejo del jugador


// Funciones de control de velocidad
void speedUp();
void speedDown();

// Funciones para actualizar propiedades de los ladrillos
void update_brick_ball(int i, int j);
void update_brick_life(int i, int j);
void update_brick_doubleRacket(int i, int j);
void update_brick_halfRacket(int i, int j);
void update_brick_speedUp(int i, int j);
void update_brick_speedDown(int i, int j);

// Función para actualizar la puntuación del jugador
void update_player_score(int brickx, int bricky);

#endif //POWERHANDLER_H
