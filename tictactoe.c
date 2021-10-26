#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tictactoe.h"


Cell Cell_create(int row, int col) {
    Cell cl;
    cl.row = row;
    cl.col = col;
    return cl;
}


int randrange(int a, int b) {
    return rand() % (b - a + 1) + a;
}


char field2char(enum field fld) {  // get char representation for field
    switch (fld) {
        case CROSS: return 'X'; break;
        case ZERO: return 'O'; break;
        default: return ' '; break;
    }
}

enum field char2field(char ch) {
    switch (ch) {
        case 'X': return CROSS; break;
        case 'O': return ZERO; break;
        default: return EMPTY; break;
    }
}


enum field check_winner(enum field map[SIZE][SIZE]) {
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
    if (map[0][0] == map[1][1] && map[1][1] == map[2][2] ||
		map[0][2] == map[1][1] && map[1][1] == map[2][0])  // diagonal check
    {
        if (map[1][1] != EMPTY) {
            return map[1][1];
        }
    }
    return EMPTY;  // winner not found
}

enum bool is_draw(enum field map[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (map[i][j] == EMPTY) {
                return FALSE;
            }
        }
    }
    return TRUE;
}   

void print_map(enum field map[SIZE][SIZE]) {  // pretty print for map
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

Cell ai_rand_turn(enum field map[SIZE][SIZE]) {
    enum bool flag = FALSE;
    int cell_num, rw, cl;
    do {
        cell_num = randrange(0, SIZE*SIZE - 1);
        rw = cell_num / SIZE;
        cl = cell_num % SIZE;
        flag = (map[rw][cl] == EMPTY);
    } while (!flag);
    return Cell_create(rw, cl);
}

InputCode check_input(enum field map[SIZE][SIZE], Cell turn) {
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

enum field switch_player(enum field current) {
    return (current == CROSS) ? ZERO : CROSS;
}
