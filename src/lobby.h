#ifndef __LOBBY_H
#define __LOBBY_H

#include "tictactoe.h"
#include "game.h"


typedef struct Lobby {
    time_t lobby_id;
} Lobby;


void check_lobby_path();
int try_get_lobby(Lobby *lobby);
int create_new_lobby(Lobby *lobby);


#endif
