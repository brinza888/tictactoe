#ifndef MINIMAX
#define MINIMAX

#include "tictactoe.h"

#define MODE_EASY 1
#define MODE_MEDIUM 2
#define MODE_HARD 3
#define MODE_EXPERT 0


struct cell {
    int row;
    int col;
};

struct cell new_cell(int row, int col);

int getMode();
void setMode(int value);

int get_moves(enum field map[SIZE][SIZE], struct cell** moves);
int minimax(enum field (*map)[SIZE][SIZE], int* row, int* cell, enum field player, int depth);

#endif
