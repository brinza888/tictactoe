#ifndef __GAME_H
#define __GAME_H

#include "tictactoe.h"


typedef int (*FHostTurn)(Game*, Cell);
typedef Cell (*FOthrTurn)(Game*);


typedef struct _GameLoop {
    WINDOW* window;
    Game *game;
    Player host_player;  // who is host player

    bool running;
    
    FHostTurn host_turn;  // function to make turn as host
    FOthrTurn othr_turn;  // function to wait turn from other

} GameLoop;


GameLoop *create_gloop(Game *game, Player host_player);
void set_host_turn(GameLoop *gloop, FHostTurn host_turn);
void set_othr_turn(GameLoop *gloop, FOthrTurn othr_turn);

int run_gloop(GameLoop *gloop);
void destroy_gloop(GameLoop *gloop);


#endif

