#ifndef __LOOPBACK_H
#define __LOOPBACK_H

#include "tictactoe.h"
#include "game.h"

void init_loopback();
void close_loopback();

int host_game();
int join_game();

int loopback_turn(GameLoop *gloop, Cell *turn);
int loopback_other_turn(GameLoop *gloop, Cell *turn);

#define LB_TURN ((TurnFunc) loopback_turn)
#define LB_OTHER_TURN ((TurnFunc) loopback_other_turn)

#endif
