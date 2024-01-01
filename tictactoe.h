#ifndef TICTACTOE
#define TICTACTOE

#include <stdbool.h>

#include "utils.h"

// size of game map (cant be changed)
#define MAP_SIZE 3
#define WIN_LINE 3

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
    Cell last_turn;
} Game;

const char *player_name(Player player);
Player switch_player(Player current);
bool is_on_map(int row, int col);

Map create_map();
void destroy_map(Map map);

Game *create_game(Player player);
void destroy_game(Game *game);

Player check_winner(const Map map, Cell last_turn);
bool check_draw(const Map map);

int check_turn(const Game *game, Cell turn);
int make_turn(Game *game, Cell turn);
void finalize_turn(Game *game);

#endif

