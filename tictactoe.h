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
} FieldType;

// cell structure
typedef struct {
    int row;
    int col;
} Cell;


// Cell functions
Cell Cell_create(int row, int col);

// service functions
int randrange(int a, int b);

// FieldType functions
char field2char(enum field fld);
enum field char2field(char ch);
enum field switch_player(enum field current);

// map functions
void print_map(enum field map[SIZE][SIZE]);

// checks functions
enum field check_winner(enum field map[SIZE][SIZE]);
bool is_draw(enum field map[SIZE][SIZE]);

// turns functions
Cell ai_rand_turn(enum field map[SIZE][SIZE]);
InputCode check_input(enum field map[SIZE][SIZE], Cell turn);

#endif
