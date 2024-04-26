#ifndef __LOBBY_H
#define __LOBBY_H

#include "tictactoe.h"
#include "game.h"

#define MAX_LOBBIES 64
#define MAX_PLAYER_NAME 64
#define MAX_SHMEM_FILE 256

typedef enum LobbyStatus {
    LS_CLOSE = 0,
    LS_OPEN = 1,
    LS_WAIT = 2,
    LS_GAME = 4
} LobbyStatus;

typedef struct Lobby {
    size_t id;
    LobbyStatus status;

    char player_cross[MAX_PLAYER_NAME];
    char player_zero[MAX_PLAYER_NAME];

    char shmem_file[MAX_SHMEM_FILE];

} Lobby;

typedef struct LobbyManager {
    Lobby[MAX_LOBBIES] lobbies;
} LobbyManager;


LobbyManager *lmgr_get();
bool lmgr_need_remove(LobbyManager *lmgr);
void lmgr_free(LobbyManager *lmgr);


Lobby *get_lobbies(LobbyManager *lmgr, int status);


#endif
