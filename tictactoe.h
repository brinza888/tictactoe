#ifndef TICTACTOE
#define TICTACTOE

#include <stdbool.h>

#include "utils.h"

// size of game map (cant be changed)
#define MAP_SIZE 3

#define E_NOTINMAP  888001
#define E_NOTEMPTY  888002


typedef enum _FieldT {
    EMPTY,
    CROSS,
    ZERO
} FieldT;

typedef FieldT Player;
typedef FieldT **Map;

typedef struct _Game {
    Map map;
    bool is_active;
    bool is_draw;
    bool is_turn_done;
    Player player;
    Player winner;
    int turn_count;
} Game;

const char *player_name(Player player);

char field2char(FieldT field);
FieldT char2field(char ch);
Player switch_player(Player current);

Game *create_game(Player player);
void destroy_game(Game *game);
void print_map(const Map map);

Player check_winner(const Map map);
bool check_draw(const Map map);

int check_turn(const Game *game, Cell turn);

int make_turn(Game *game, Cell turn);
void finalize_turn(Game *game);

#endif

