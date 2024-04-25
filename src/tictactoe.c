#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#include "tictactoe.h"


const char *player_name(Player player) {
    static char name[32];
    switch (player) {
    case CROSS:
        strcpy(name, "Cross"); break;
    case ZERO:
        strcpy(name, "Zero"); break;
    default:
        strcpy(name, "-"); break;
    }
    return (const char *) name;
}

Player switch_player(Player current) {
    return (current == CROSS) ? ZERO : CROSS;
}

bool is_on_map(int row, int col) {
    return (row >= 0 && row < MAP_SIZE) && (col >= 0 && col < MAP_SIZE);
}

Map create_map() {
    Map map = calloc(sizeof(FieldT*), MAP_SIZE);
    for (size_t i = 0; i < MAP_SIZE; i++) {
        map[i] = calloc(sizeof(FieldT), MAP_SIZE);
        for (size_t j = 0; j < MAP_SIZE; j++) {
            map[i][j] = EMPTY;
        }
    }
    return map;
}

void destroy_map(Map map) {
    for (size_t i = 0; i < MAP_SIZE; i++) {
        free(map[i]);
    }
    free(map);
}

Game *create_game(Player player) {
    Game *game = calloc(sizeof(Game), 1);
    game->map = create_map();
    game->player = player;
    game->turn_count = 0;
    game->winner = EMPTY;
    game->is_active = true;
    game->is_turn_done = false;
    game->is_draw = false;
    game->last_turn = (Cell) {-1, -1};
    return game;
}

void destroy_game(Game *game) {
    destroy_map(game->map);
    free(game);
}

Player check_winner(const Map map, Cell last_turn) {
    int lrow = last_turn.row;
    int lcol = last_turn.col;

    Player player = map[lrow][lcol];

    int win_s = -WIN_LINE + 1;
    int win_e = 0;
    bool result = false;

    for (int step = 0; step < WIN_LINE; step++) {
        if (is_on_map(lrow + win_s, lcol) && is_on_map(lrow + win_e, lcol)) {
            result = true;
            for (int row = lrow + win_s; row <= lrow + win_e; row++)
                result = result && (map[row][lcol] == player);
            if (result)
                return player;
        }
        if (is_on_map(lrow, lcol + win_s) && is_on_map(lrow, lcol + win_e)) {
            result = true;
            for (int col = lcol + win_s; col <= lcol + win_e; col++)
                result = result && (map[lrow][col] == player);
            if (result)
                return player;
        }
        if (is_on_map(lrow + win_s, lcol + win_s) && is_on_map(lrow + win_e, lcol + win_e)) {
            result = true;
            for (int i = win_s; i <= win_e; i++)
                result = result && (map[lrow + i][lcol + i] == player);
            if (result)
                return player;
        }
        if (is_on_map(lrow + win_s, lcol - win_s) && is_on_map(lrow + win_e, lcol - win_e)) {
            result = true;
            for (int i = win_s; i <= win_e; i++)
                result = result && (map[lrow + i][lcol - i] == player);
            if (result)
                return player;
        }
        win_s += 1;
        win_e += 1;
    }

    return EMPTY;
}

bool check_draw(const Map map) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (map[i][j] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}

int check_turn(const Game *game, Cell turn) {
    if (!is_on_map(turn.row, turn.col))
    {
        return E_NOTINMAP;
    }
    if (game->map[turn.row][turn.col] != EMPTY) {  // check if cell already taken
        return E_NOTEMPTY;
    } else {
        return 0;
    }
}

int make_turn(Game *game, Cell turn) {
    int check_res;
    if ((check_res = check_turn(game, turn)) != 0)
        return check_res;
    game->is_turn_done = true;
    game->map[turn.row][turn.col] = game->player;
    game->last_turn = turn;
    return 0;
}

void finalize_turn(Game *game) {
    if (!game->is_turn_done)
        return;
    game->is_turn_done = false;
    game->turn_count += 1;
    game->winner = check_winner(game->map, game->last_turn);
    game->is_draw = check_draw(game->map);
    game->player = switch_player(game->player);
}
