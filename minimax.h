#ifndef MINIMAX
#define MINIMAX

#include "tictactoe.h"

struct cell {
    int row;
    int col;
};

struct cell new_cell(int row, int col);

int get_moves(enum field map[SIZE][SIZE], struct cell** moves);
int minimax(enum field (*map)[SIZE][SIZE], int* row, int* cell, enum field player, int depth);

#endif
