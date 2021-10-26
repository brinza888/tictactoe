#ifndef TICTACTOE
#define TICTACTOE

#include <stdbool.h>

// size of game map
#define SIZE 3  // cant be changed!

// error codes for check_input
typedef enum InputCode {
    INPUT_OK,
    NOTONMAP,
    NOTEMPTY
} InputCode;


// cell type or player type
typedef enum field {
    EMPTY,
    CROSS,
    ZERO
} FieldT;

// cell structure
typedef struct {
    int row;
    int col;
} Cell;


// Cell functions
Cell Cell_create(int row, int col);

// FieldT functions
char field2char(FieldT fld);
FieldT char2field(char ch);
FieldT switch_player(FieldT current);

// map functions
void print_map(FieldT map[SIZE][SIZE]);

// check functions
FieldT check_winner(FieldT map[SIZE][SIZE]);
bool is_draw(FieldT map[SIZE][SIZE]);

// check posibility of turn
InputCode check_input(FieldT map[SIZE][SIZE], Cell turn);

#endif

