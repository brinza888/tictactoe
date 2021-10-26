#include <stdlib.h>
#include <stdio.h>

#include "tictactoe.h"
#include "minimax.h"


static int depthMode = MODE_EXPERT;

int getMode() {
    return depthMode;
}

void setMode(int value) {
    depthMode = value;
}

int getMoves(FieldT map[SIZE][SIZE], Cell** moves, size_t* n) {
    if (*n < 1 || *moves == NULL) {
        *n = sizeof(Cell);
        *moves = (Cell*) malloc(*n);
    }
    int count = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (map[i][j] != EMPTY) {
                continue;
            }
            count++;
            if (*n < count * sizeof(Cell)) {
                *n += sizeof(Cell);
                *moves = (Cell*) realloc(*moves, *n);
                if (!moves) {
                    fprintf(stderr, "Not enough memory!\n");
                    exit(1);
                }
            }
            (*moves)[count - 1] = Cell_create(i, j);
        }
    }
    return count;
}


int minimax(FieldT (*map)[SIZE][SIZE], Cell* turn, FieldT player, int depth) {
    turn->row = -1;
    turn->col = -1;
    
    FieldT winner = check_winner(*map);
    if (winner != EMPTY) {
        return (winner == ZERO) ? 10 : -10;
    } 
    if (is_draw(*map) || (depthMode != 0 && depth > depthMode)) {
        return 0;
    }
    
    Cell* moves;
    int count = 0;
    size_t allocated = 0;
    count = getMoves(*map, &moves, &allocated);

    Cell* ch_move;
    int mnmx = (player == ZERO) ? -20 : 20;  // optimized value
    int res = 0;  // last minimax value
    int rw, cl;  // make move
    
    for (int i = 0; i < count; i++) {
        rw = moves[i].row;
        cl = moves[i].col;

        (*map)[rw][cl] = player;

        res = minimax(map, turn, switch_player(player), depth + 1);

        if ((player == ZERO && res > mnmx) || (player == CROSS && res < mnmx)) {
            mnmx = res;
            ch_move = moves + i;
        }
        
        (*map)[rw][cl] = EMPTY;
    }

    turn->row = ch_move->row;
    turn->col = ch_move->col;
    
    free(moves);    

    return mnmx;
}

