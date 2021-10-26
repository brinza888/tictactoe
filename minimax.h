#ifndef MINIMAX
#define MINIMAX

#include "tictactoe.h"

#define MODE_EASY 1
#define MODE_MEDIUM 2
#define MODE_HARD 3
#define MODE_EXPERT 0


// random integer from a to b
int randrange(int a, int b);

// minimax depth control
int getMode();
void setMode(int value);

// service functions
int getMoves(FieldT (*map)[SIZE][SIZE], Cell** moves, size_t* n);

// turns functions
int ai_minimax(FieldT (*map)[SIZE][SIZE], Cell* turn, FieldT player, int depth);
void ai_random(FieldT (*map)[SIZE][SIZE], Cell* turn);

#endif

