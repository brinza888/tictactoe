#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "tictactoe.h"


Cell Cell_create(int row, int col) {
    Cell cl;
    cl.row = row;
    cl.col = col;
    return cl;
}


char field2char(FieldT fld) {  // get char representation for field
    switch (fld) {
        case CROSS: return 'X'; break;
        case ZERO: return 'O'; break;
        default: return ' '; break;
    }
}


FieldT char2field(char ch) {
    switch (ch) {
        case 'X': return CROSS; break;
        case 'O': return ZERO; break;
        default: return EMPTY; break;
    }
}


FieldT check_winner(FieldT map[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {  // horizontal lines
        if (map[i][0] == map[i][1] && map[i][1] == map[i][2] && map[i][1] != EMPTY) {
            return map[i][1];
        }
    }
    for (int i = 0; i < SIZE; i++) {  // vertical lines
        if (map[0][i] == map[1][i] && map[1][i] == map[2][i] && map[1][i] != EMPTY) {
            return map[1][i];
        }
    }
    if ((map[0][0] == map[1][1] && map[1][1] == map[2][2]) ||
		(map[0][2] == map[1][1] && map[1][1] == map[2][0]))  // diagonal check
    {
        if (map[1][1] != EMPTY) {
            return map[1][1];
        }
    }
    return EMPTY;  // winner not found
}


bool is_draw(FieldT map[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (map[i][j] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}   


void print_map(FieldT map[SIZE][SIZE]) {  // pretty print for map
    printf("  ");
    for (int i = 0; i < SIZE; i++) {  // columns indices
        printf(" %d", i + 1);
    }
    printf("\n  ");
    for (int k = 0; k < SIZE; k++) {  // top border
        printf("+-");
    }
    printf("+\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d |", i + 1);
        for (int j = 0; j < SIZE; j++) {
            printf("%c|", field2char(map[i][j]));
        }
        printf("\n  ");
        for (int k = 0; k < SIZE; k++) {  // interior border
            printf("+-");
        }
        printf("+\n");
    }
}


InputCode check_input(FieldT map[SIZE][SIZE], Cell turn) {
    if (turn.row > SIZE - 1 || turn.row < 0 ||
        turn.col > SIZE - 1 || turn.col < 0)
    {
        return NOTONMAP;
    }
    if (map[turn.row][turn.col] != EMPTY) {  // check if cell already taken
        return NOTEMPTY;
    } else {
        return INPUT_OK;
    }
}


FieldT switch_player(FieldT current) {
    return (current == CROSS) ? ZERO : CROSS;
}

