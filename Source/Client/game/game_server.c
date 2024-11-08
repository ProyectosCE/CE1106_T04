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

// BIBLIOTECAS EXTERNAS
#include <string.h>  // Funciones para manipulación de cadenas (e.g., strcmp, strcpy, etc.).
#include <stdio.h>   // Funciones de entrada y salida estándar (e.g., printf, scanf, etc.).
#include <stdlib.h>  // Funciones generales de propósito, como gestión de memoria (e.g., malloc, free, etc.).
#include <unistd.h>  // Funciones para operaciones del sistema POSIX (e.g., sleep, close, etc.).

// BIBLIOTECAS DE PROYECTO
#include "game_logic.h"       // Lógica principal del juego.
#include "../gui/main_menu.h" // Funciones para el menú principal de la interfaz gráfica.
#include "spectator.h"        // Manejador de espectadores en el juego.
#include "Objects/ball.h"     // Implementación y lógica de las pelotas del juego.
#include "Objects/brick.h"    // Implementación y lógica de los ladrillos.
#include "Objects/player.h"   // Implementación y lógica del jugador.
#include "powerHandler.h"     // Manejo de poderes y bonificaciones.
#include "../gui/screenHandler.h" // Manejo de pantallas de la interfaz gráfica.

// Variables Globales
pthread_t askForUserThread; // Hilo para manejar la interacción con el usuario.

/* Function: init_game_server
   Descripción:
     Inicializa el servidor del juego configurando el estado inicial del juego,
     incluyendo al jugador, las pelotas, los ladrillos y los manejadores de poder.

   Params:
     (Ninguno)

   Returns:
     - void: Esta función no devuelve valores.

   Restriction:
     - Esta función debe ser llamada antes de iniciar la lógica principal del juego para garantizar
       que el estado inicial esté correctamente configurado.

   Example:
     init_game_server();
     // Configura el juego con los valores iniciales antes de entrar en el bucle principal.

   Problems:
     - Problema: La inicialización de tamaños de ladrillos puede depender de las dimensiones de la pantalla.
       - Solución: Asegurarse de que las dimensiones de la pantalla (screenWidth, screenHeight) se configuren
         antes de llamar a esta función.
*/

void init_game_server() {
    GameState *gameStatus = getGameState(); // Obtiene el estado global del juego.

    // Inicializa al jugador con la vida máxima configurada.
    init_player(&gameStatus->player, gameStatus->playerMaxLife);

    // Inicializa las pelotas en el estado inicial.
    init_balls(gameStatus->balls);

    // Configura el tamaño de los ladrillos en función de las dimensiones de la pantalla y el número de ladrillos.
    brickSize = (Vector2){
        screenWidth / gameStatus->bricksPerLine,
        screenHeight / (gameStatus->linesOfBricks * 2)
    };
    gameStatus->brickSize = brickSize;

    // Inicializa los ladrillos en el estado inicial del juego.
    init_bricks(gameStatus, gameStatus->brickSize);

    // Inicializa el manejador de poderes (bonificaciones).
    initPowerHandler();

    // Configura los valores iniciales del estado del juego.
    gameStatus->restart = false;
    gameStatus->pause = false;
    gameStatus->winner = false;
    gameStatus->gameOver = false;

}

/* Function: gameServerCallback
   Descripción:
     Procesa los mensajes recibidos del servidor del juego, actualizando el estado de los ladrillos.

   Params:
     recibido - Mensaje recibido como una cadena de caracteres que contiene información sobre
                los ladrillos a procesar.

   Returns:
     - void: Esta función no devuelve valores.

   Restriction:
     - El mensaje recibido debe estar en un formato válido para ser procesado por process_brick_update.

   Example:
     const char *mensaje = "BrickUpdate:...";
     gameServerCallback(mensaje);

*/

void gameServerCallback(const char *recibido) {
    process_brick_update(recibido); // Procesa las actualizaciones de ladrillos a partir del mensaje recibido.
}

/* Function: updateNameInput
   Descripción:
     Gestiona la entrada del nombre del jugador, permitiendo escribir, borrar y confirmar el nombre
     para enviarlo al servidor y cambiar la pantalla al juego.

   Params:
     game_state - Puntero al estado global del juego que contiene el nombre del jugador y otras configuraciones.

   Returns:
     - void: Esta función no devuelve valores.

   Restriction:
     - La función asume que el nombre del jugador está contenido en game_state->playerName y tiene un tamaño fijo.
     - Las teclas deben estar correctamente mapeadas a valores entre KEY_A y KEY_Z.

   Example:
     updateNameInput(gameState);
     // Permite al usuario ingresar su nombre antes de iniciar el juego.

   Problems:
     - Problema: Si el buffer de entrada no es lo suficientemente largo, podría provocar un desbordamiento.
       - Solución: Garantizar que game_state->playerName tenga el tamaño adecuado y validar la longitud.

   References:
   - Santamaria, R. (2017-2024). raylib [text] example - Input Box. raylib. Recuperado de https://www.raylib.com/examples/text/loader.html?name=text_input_box

*/
void updateNameInput(GameState *game_state) {
    if (IsKeyPressed(KEY_ENTER)) {
        // Si se presiona Enter, se crea el servidor de comunicación, se envía el nombre del jugador
        // y se inicializa el juego.
        game_state->comServer = ComServer_create();
        ComServer_sendPlayerName(game_state->comServer, game_state->playerName);
        init_game_server();
        setCurrentScreen(GAME); // Cambia la pantalla actual al juego.
    } else if (IsKeyPressed(KEY_BACKSPACE) && strlen(game_state->playerName) > 0) {
        // Si se presiona Backspace, elimina el último carácter del nombre.
        game_state->playerName[strlen(game_state->playerName) - 1] = '\0';
    } else {
        // Si se presiona una letra (A-Z), la agrega al nombre del jugador si hay espacio disponible.
        for (int key = KEY_A; key <= KEY_Z; key++) {
            if (IsKeyPressed(key) && strlen(game_state->playerName) < sizeof(game_state->playerName) - 1) {
                game_state->playerName[strlen(game_state->playerName)] = (char)(key + 'A' - KEY_A);
                game_state->playerName[strlen(game_state->playerName) + 1] = '\0'; // Termina la cadena.
            }
        }
    }
}



// ================================================================================================================= //
// COMUNICACION THREAD
/* Function: initialize_game_communication
   Descripción:
     Inicializa la comunicación del juego configurando el servidor de comunicación (ComServer)
     y creando un hilo para escuchar mensajes. Si ya existe una instancia de ComServer, no se vuelve a crear.

   Params:
     gameState - Puntero al estado global del juego que contiene la configuración actual del juego,
                 incluida la referencia al servidor de comunicación.
     callback  - Puntero a una función que se ejecutará como callback cuando se reciban mensajes.

   Returns:
     - void: Esta función no devuelve valores.

   Restriction:
     - Esta función debe llamarse antes de iniciar la lógica principal del juego.
     - El estado del juego (`gameState`) debe estar correctamente inicializado antes de usar esta función.

   Example:
     initialize_game_communication(&gameState, callbackFunction);
     // Configura la comunicación del juego con un callback para procesar mensajes.

   Problems:
     - Problema: Si no se puede crear el ComServer o el hilo de comunicación, el juego no podrá continuar.
       - Solución: Se imprime un mensaje de error en la salida estándar de error (stderr)
         y se detiene el estado de ejecución (`gameState->running = false`).

   References:
     - Kerrisk, M. (2024). HTML rendering of pthread_create(3).
        Man7.org. Recuperado de https://man7.org/linux/man-pages/man3/pthread_create.3.html
*/

void initialize_game_communication(GameState *gameState, void (*callback)(void *)) {
    // Si el servidor de comunicación no ha sido creado, se crea.
    if (gameState->comServer == NULL) {
        gameState->comServer = ComServer_create(); // Crea una nueva instancia de ComServer.
        if (gameState->comServer == NULL) {
            // Si la creación falla, muestra un mensaje de error y detiene el juego.
            fprintf(stderr, "Error al crear ComServer\n");
            gameState->running = false;
            return;
        }
        // Registra el callback para manejar mensajes entrantes.
        ComServer_registerCallback(gameState->comServer, callback);
    }

    // Crea un hilo para escuchar los mensajes del servidor de comunicación.
    if (pthread_create(
            &gameState->communicationThread,
            NULL,
            ComServer_messageListeningLoop,
            (void *)gameState->comServer) != 0) {
        // Si la creación del hilo falla, muestra un mensaje de error, destruye el ComServer y detiene el juego.
        fprintf(stderr, "Error al crear el hilo de comunicación\n");
        ComServer_destroy(gameState->comServer); // Libera recursos del servidor de comunicación.
        gameState->running = false; // Indica que el juego no está en ejecución.
    }
}


// ============================================================================================================== //
// UPDATE GAME STATE
/* Function: update_game_state
   Descripción:
     Actualiza el estado del juego basado en la pantalla actual, gestionando las transiciones entre pantallas,
     la inicialización de la comunicación y la lógica principal del juego.

   Params:
     gameState - Puntero al estado global del juego que contiene toda la información relevante para
                 gestionar las pantallas y la lógica del juego.

   Returns:
     - void: Esta función no devuelve valores.

   Restriction:
     - El estado global del juego debe estar correctamente inicializado antes de llamar a esta función.
     - Las transiciones entre pantallas deben manejarse según las reglas establecidas en el juego.

   Example:
     update_game_state(&gameState);
     // Se utiliza en el bucle principal del juego para actualizar su estado.

   Problems:
     - Problema: Si ocurre un error al crear hilos para comunicación o estados, el juego no puede continuar.
       - Solución: Se registran errores en stderr y se detiene la ejecución (`gameState->running = false`).

   References:
    - Kerrisk, M. (2024). HTML rendering of pthread_create(3).
        Man7.org. Recuperado de https://man7.org/linux/man-pages/man3/pthread_create.3.html
*/

void update_game_state(GameState *gameState) {
    // Determina la acción según la pantalla actual
    switch (getCurrentScreen()) {
        case MENU:
            // Actualiza el menú principal
            UpdateMenu();
            break;

        case NAME_INPUT:
            // Maneja la entrada del nombre del jugador
            updateNameInput(gameState);
            break;

        case GAME:
            if (!gameState->comunicationRunning) {
                // Inicializa la comunicación del juego si aún no está activa
                gameState->comunicationRunning = true;
                printf("COMUNICACIONES INICIADAS EN GAMEE!");
                gameState->comServer = NULL;

                // Inicializa el servidor de comunicación y el hilo para enviar estados del juego
                initialize_game_communication(gameState, gameServerCallback);

                if (pthread_create(&gameState->sendStatusThread, NULL, send_game_state_thread, (void *)gameState) != 0) {
                    fprintf(stderr, "Error al crear el hilo de envío de estado\n");
                    ComServer_destroy(gameState->comServer);
                    gameState->running = false;
                    gameState->comunicationRunning = false;
                    return;
                }
            }

            // Actualiza la lógica principal del juego
            update_game(gameState);

            if (gameState->winner && gameState->restart) {
                init_game_server();
                gameState->ball_speed_multiplier+=0.25f;
                gameState->levelsCompleted+=1;
            }
            // Reinicia el juego si se solicita
            if (gameState->restart) {
                init_game_server();
                gameState->ball_speed_multiplier=1.0f;
                gameState->levelsCompleted=0;
            }
            break;

        case OBSERVER_SELECT: {
            if (!gameState->comunicationRunning) {
                // Inicializa la comunicación para el modo observador
                initialize_game_communication(gameState, espectadorGetList);

                init_game_server();
                gameState->running = true;
                gameState->comunicationRunning = true;

                // Crea el hilo para solicitar la lista de jugadores
                if (pthread_create(&gameState->askForUsersThread, NULL, askForPlayerList, (void *)gameState) != 0) {
                    fprintf(stderr, "Error al crear el hilo de solicitud de jugadores\n");
                    ComServer_destroy(gameState->comServer);
                    gameState->comunicationRunning = false;
                    gameState->running = false;
                    return;
                }
            }

            // Actualiza la lista de jugadores si está disponible
            PlayerList *playerLista = GetPlayerListInstance();
            if (playerLista != NULL) {
                UpdatePlayerList(playerLista);
            }
            break;
        }

        case SPECTATOR:
            if (!gameState->comunicationRunning) {
                // Inicializa la comunicación para el espectador
                init_game_server();
                gameState->running = true;
                gameState->comunicationRunning = true;
                gameState->comServer = NULL;

                initialize_game_communication(gameState, espectadorUpdateGame);
            }
            break;

        default:
            break;
    }

    // Verifica si el jugador ha perdido todas las vidas
    if (gameState->player.life <= 0) {
        gameState->gameOver = true;
    }
}



// ============================================================================================================= //

// UPDATE THREAD

/* Function: update_thread
   Descripción:
     Hilo encargado de actualizar el estado del juego de forma continua mientras el juego está en ejecución.
     La función bloquea el mutex global durante la actualización para garantizar la sincronización entre hilos.

   Params:
     arg - Puntero al estado global del juego (`GameState *`), que contiene toda la información necesaria
           para actualizar el estado del juego.

   Returns:
     - void*: Devuelve `NULL` al finalizar la ejecución del hilo.

   Restriction:
     - Se debe garantizar que el `GameState` pasado como argumento esté correctamente inicializado.
     - El mutex `gameStateMutex` debe estar definido globalmente y utilizado para sincronizar el acceso.

   Example:
     pthread_t gameThread;
     pthread_create(&gameThread, NULL, update_thread, (void *)&gameState);
     // Crea un hilo para actualizar el estado del juego.

   Problems:
     - Problema: Si el mutex no se desbloquea correctamente, podría causar un bloqueo (deadlock).
       - Solución: Asegurarse de que todas las rutas de ejecución liberen el mutex antes de salir.

   References:
     - pthread_mutex_lock: https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html
     - usleep: https://man7.org/linux/man-pages/man3/usleep.3.html
*/

void *update_thread(void *arg) {
    GameState *gameState = (GameState *)arg; // Castea el argumento a GameState.

    while (gameState->running) { // Bucle principal: el hilo se ejecuta mientras el juego está activo.
        pthread_mutex_lock(&gameStateMutex); // Bloquea el mutex para sincronizar el acceso al estado del juego.

        // Actualiza el estado del juego según la pantalla actual.
        update_game_state(gameState);

        pthread_mutex_unlock(&gameStateMutex); // Desbloquea el mutex tras la actualización.

        // Pausa para limitar la velocidad de actualización (~60 FPS).
        usleep(16000);
    }

    return NULL; // Finaliza la ejecución del hilo devolviendo NULL.
}



// ============================================================================================================= //

// START GAME

/* Function: start_game
   Descripción:
     Inicia el juego configurando el estado inicial, inicializando el mutex global y creando los hilos
     principales para la actualización y el dibujo del juego. Espera la finalización de los hilos antes de limpiar recursos.

   Params:
     (Ninguno)

   Returns:
     - void: Esta función no devuelve valores.

   Restriction:
     - El `GameState` debe estar correctamente inicializado antes de llamar a esta función.
     - Se requiere soporte para `pthread` en el entorno de compilación y ejecución.

   Example:
     start_game();
     // Llama a esta función para iniciar el juego desde el flujo principal.

   Problems:
     - Problema: Si falla la creación del mutex o de los hilos, el juego no podrá continuar.
       - Solución: Se manejan errores con mensajes en `stderr` y se asegura la limpieza de recursos.
     - Problema: Deadlock si no se desbloquean los mutex correctamente.
       - Solución: Uso adecuado de `pthread_mutex_lock` y `pthread_mutex_unlock`.

   References:
     - pthread_create: https://man7.org/linux/man-pages/man3/pthread_create.3.html
     - pthread_mutex_init: https://man7.org/linux/man-pages/man3/pthread_mutex_init.3p.html
     - pthread_join: https://man7.org/linux/man-pages/man3/pthread_join.3p.html
*/

void start_game() {
    GameState *gameState = getGameState(); // Obtiene el estado global del juego.
    pthread_t updateThread, drawThread;   // Declaración de hilos para actualizar y dibujar el juego.

    // Inicializa el mutex global para proteger el acceso al estado del juego.
    if (pthread_mutex_init(&gameStateMutex, NULL) != 0) {
        fprintf(stderr, "Error al inicializar el mutex\n");
        return;
    }

    // Configura el estado inicial del juego.
    gameState->running = true;

    // Crea e inicia el hilo para la actualización del estado del juego.
    if (pthread_create(&updateThread, NULL, update_thread, (void *)gameState) != 0) {
        fprintf(stderr, "Error al crear el thread de actualización\n");
        gameState->running = false;
        return;
    }

    // Crea e inicia el hilo para dibujar el juego en pantalla.
    if (pthread_create(&drawThread, NULL, draw_thread, (void *)gameState) != 0) {
        fprintf(stderr, "Error al crear el thread de dibujo\n");
        gameState->running = false;
        return;
    }

    // Espera a que los hilos terminen (sincronización).
    pthread_join(updateThread, NULL); // Espera la finalización del hilo de actualización.
    pthread_join(drawThread, NULL);  // Espera la finalización del hilo de dibujo.

    // Destruye el mutex al finalizar para liberar recursos.
    pthread_mutex_destroy(&gameStateMutex);
}
