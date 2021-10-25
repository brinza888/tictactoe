#ifndef TICTACTOE
#define TICTACTOE

// size of game map
#define SIZE 3  // cant be changed!

// error codes for input_cell
#define OK 0
#define NOTONMAP 1
#define NOTEMPTY 2

// boolean values
enum bool {
    FALSE,
    TRUE
};

// cell type or player type
enum field {
    EMPTY,
    CROSS,
    ZERO
};

// cell structure
typedef struct {
    int row;
    int col;
} Cell;

// service functions
int randrange(int a, int b);
Cell* new_cell(int row, int col);

char field2char(enum field fld);
enum field char2field(char ch);

enum field switch_player(enum field current);

void print_map(enum field map[SIZE][SIZE]);

// checks functions
enum field check_winner(enum field map[SIZE][SIZE]);
enum bool is_draw(enum field map[SIZE][SIZE]);

// turns functions
Cell* ai_rand_turn(enum field map[SIZE][SIZE]);
int check_input(enum field map[SIZE][SIZE], int row, int col);

#endif
