#ifndef __GAME_H
#define __GAME_H

#include <ncurses.h>

#include "tictactoe.h"
#include "utils.h"

typedef int (*TurnFunc)(void*, Cell*);

typedef struct _GameLoop {
    WINDOW* window;
    Game *game;
    Player host_player;  // who is host player
    Player othr_player;  // who is opponent of host player

    bool running;
    bool enter_hit;
    Selection *row;
    Selection *col;
    
    TurnFunc host_turn;  // function to make turn as host
    TurnFunc othr_turn;  // function to wait turn from other

} GameLoop;


GameLoop *create_gloop(Game *game, Player host_player);
void set_host_turn(GameLoop *gloop, TurnFunc host_turn);
void set_othr_turn(GameLoop *gloop, TurnFunc othr_turn);

int run_gloop(GameLoop *gloop);
void destroy_gloop(GameLoop *gloop);

int keyboard_turn(GameLoop *gloop, Cell *turn);


#endif

