#ifndef CLIENT_SPECTATOR_H
#define CLIENT_SPECTATOR_H

#include "raylib.h"

typedef struct {

    char playerNames[10][50]; // Suponiendo un máximo de 10 jugadores y nombres de hasta 50 caracteres

    char playerUUIDs[10][37]; // UUIDs son de 36 caracteres + 1 para el terminador

    int numPlayers;

} PlayerList;

void DrawPlayerList();
void *askForPlayerList(void *arg);
void UpdatePlayerList(const PlayerList *players);
void espectadorGetList(const char* recibido);
void espectadorUpdateGame(const char *recibido);
PlayerList* GetPlayerListInstance();

//void DrawGameSpectator();
//void parseGameState();

// Cambiar despues para que reciba json
//char* readJSONFile(const char *filename);


#endif //CLIENT_SPECTATOR_H
