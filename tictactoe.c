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

char field2char(FieldT field) {
    switch (field) {
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

Player switch_player(Player current) {
    return (current == CROSS) ? ZERO : CROSS;
}

bool check_cell(Cell cell) {
    return cell.row < MAP_SIZE && cell.row >= 0 && cell.col < MAP_SIZE && cell.col >= 0;
}


Game *create_game(Player player) {
    Game *game = calloc(sizeof(Game), 1);
    game->map = calloc(sizeof(FieldT*), MAP_SIZE);
    for (size_t i = 0; i < MAP_SIZE; i++) {
        game->map[i] = calloc(sizeof(FieldT), MAP_SIZE);
        for (size_t j = 0; j < MAP_SIZE; j++) {
            game->map[i][j] = EMPTY;
        }
    }
    game->player = player;
    game->turn_count = 0;
    game->winner = EMPTY;
    game->is_active = true;
    game->is_turn_done = false;
    game->is_draw = false;
    return game;
}

void destroy_game(Game *game) {
    for (size_t i = 0; i < MAP_SIZE; i++) {
        free(game->map[i]);
    }
    free(game->map);
    free(game);
}


bool check_win_lines(const Map map, int row, int col) {
    Player pl = map[row][col];
    bool h = (col + (WIN_LINE - 1) < MAP_SIZE);
    bool v = (row + (WIN_LINE - 1) < MAP_SIZE);
    bool d1 = h && v;
    bool d2 = v && (col - (WIN_LINE - 1) >= 0);

    for (int d = 0; d < WIN_LINE; d++) {
        if (h)
            h = (map[row][col + d] == pl);
        if (v)
            v = (map[row + d][col] == pl);
        if (d1)
            d1 = (map[row + d][col + d] == pl);
        if (d2)
            d2 = (map[row + d][col - d] == pl);
    }

    return h || v || d1 || d2;
}

Player check_winner(const Map map) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (map[i][j] != EMPTY) {
                bool result = check_win_lines(map, i, j);
                if (result)
                    return map[i][j];
            }
        }
    }
    return false;
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

void print_map(const Map map) {  // pretty print for map
    printf("  ");
    for (int i = 0; i < MAP_SIZE; i++) {  // columns indices
        printf(" %d", i + 1);
    }
    printf("\n  ");
    for (int k = 0; k < MAP_SIZE; k++) {  // top border
        printf("+-");
    }
    printf("+\n");
    for (int i = 0; i < MAP_SIZE; i++) {
        printf("%d |", i + 1);
        for (int j = 0; j < MAP_SIZE; j++) {
            printf("%c|", field2char(map[i][j]));
        }
        printf("\n  ");
        for (int k = 0; k < MAP_SIZE; k++) {  // interior border
            printf("+-");
        }
        printf("+\n");
    }
}

int check_turn(const Game *game, Cell turn) {
    if (!check_cell(turn))
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
    return 0;
}

void finalize_turn(Game *game) {
    if (!game->is_turn_done)
        return;
    game->is_turn_done = false;
    game->turn_count += 1;
    game->winner = check_winner(game->map);
    game->is_draw = check_draw(game->map);
    game->player = switch_player(game->player);
}
