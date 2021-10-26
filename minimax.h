#ifndef MINIMAX
#define MINIMAX

#include "tictactoe.h"

#define MODE_EASY 1
#define MODE_MEDIUM 2
#define MODE_HARD 3
#define MODE_EXPERT 0


int getMode();
void setMode(int value);

int getMoves(FieldT (*map)[SIZE][SIZE], Cell** moves, size_t* n);

int minimax(FieldT (*map)[SIZE][SIZE], Cell* turn, FieldT player, int depth);

#endif
