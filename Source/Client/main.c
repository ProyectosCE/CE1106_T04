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
//BIBLIOTECAS EXTERNAS
#include <stdlib.h>

//BIBLIOTECAS DE PROYECTO
#include "game/game_server.h"

/* Function: main
   Funcion principal de ejecución del juego

   Params:
     void - Sin parametros

   Returns:
     EXIT_SUCCESS - Retorna 0 si la ejecución fue correcta
*/

int main(void) {
    start_game();
    return EXIT_SUCCESS;
}
