#include <stdlib.h>
#include <stdio.h>

#include "tictactoe.h"
#include "minimax.h"


struct cell new_cell(int row, int col) {
    struct cell new;
    new.row = row;
    new.col = col;
    return new;
}


int get_moves(enum field map[SIZE][SIZE], struct cell **moves) {
    int count = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (map[i][j] == EMPTY) {
                count += 1;
                if (sizeof(**moves) < count * sizeof(struct cell)) {
                    *moves = (struct cell*) realloc(*moves, sizeof(struct cell) * count);
                    if (moves == NULL) {
                        printf("Cant realoc moves!\n");
                        exit(1);
                    }
                }
                (*moves)[count - 1] = new_cell(i, j);
            }         
        }
    }
    return count;
}


int minimax(enum field (*map)[SIZE][SIZE], int* row, int* col, enum field player, int depth) {
    *row = -1;
    *col = -1;
    
    enum field winner = check_winner(*map);
    if (winner != EMPTY) {
        return (winner == ZERO) ? 10 : -10;
    } 
    if (is_draw(*map)) {
        return 0;
    }

    struct cell* moves = (struct cell*) malloc(sizeof(struct cell) * 1);
    if (moves == NULL) {
        printf("Cant malloc moves!\n");
        exit(1);
    }

    int count = get_moves(*map, &moves);
    struct cell ch_move = moves[0];  // choosen move
    int mnmx = (player == ZERO) ? -20 : 20;  // optimized value
    int res = 0;  // last minimax value
    int rw, cl;  // make move
    int r, c;  // next turn move
    
    for (int i = 0; i < count; i++) {
        rw = moves[i].row;
        cl = moves[i].col;
        (*map)[rw][cl] = player;

        res = minimax(map, &r, &c, switch_player(player), depth + 1);

        if (player == ZERO) {
            if (res > mnmx) {
                mnmx = res;
                ch_move = new_cell(rw, cl);
            }
        } else if (player == CROSS) {
            if (res < mnmx) { 
                mnmx = res;
                ch_move = new_cell(rw, cl);
            }
        }
        
        (*map)[rw][cl] = EMPTY;
    }
    
    free(moves);

    *row = ch_move.row;
    *col = ch_move.col;
     
    return mnmx;
}

