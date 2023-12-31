#include <stdlib.h>
#include <stdio.h>

#include "tictactoe.h"
#include "ai.h"
#include "utils.h"


static int AI_Mode = MODE_EASY;

void set_ai_mode(int ai_mode) {
    AI_Mode = ai_mode;
}

int get_ai_mode() {
    return AI_Mode;
}

int get_moves(const Map map, Cell *moves) {
    int count = 0;
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (map[i][j] != EMPTY) {
                continue;
            }
            moves[count++] = make_cell(i, j);
        }
    }
    return count;
}

int ai_minimax(Map map, Player player, Cell *turn, int depth, int depthMax) {
    Player winner = check_winner(map);
    if (winner != EMPTY) {
        return (winner == ZERO) ? 10 : -10;
    }
    if (check_draw(map) || (depthMax != -1 && depth > depthMax)) {
        return 0;
    }
    
    Cell moves[MAP_SIZE * MAP_SIZE];
    int moves_count = get_moves(map, moves);

    int best_move;
    int mnmx = (player == ZERO) ? -20 : 20;  // optimized value
    int res = 0;  // last minimax value
    int row, col;   // make move
    
    for (int i = 0; i < moves_count; i++) {
        row = moves[i].row;
        col = moves[i].col;
        map[row][col] = player;  // asume player make turn with this move
        res = ai_minimax(map, switch_player(player), NULL, depth + 1, depthMax);
        map[row][col] = EMPTY;   // restore map to previous state
        if ((player == ZERO && res > mnmx) || (player == CROSS && res < mnmx)) {
            mnmx = res;
            best_move = i;
        }
    }

    if (turn != NULL) {
        turn->row = moves[best_move].row;
        turn->col = moves[best_move].col;
    }

    return mnmx;
}

void ai_random(Map map, Cell* turn) {
    Cell moves[MAP_SIZE * MAP_SIZE];
    int moves_count = get_moves(map, moves);
    int move = randrange(0, moves_count - 1);
    turn->row = moves[move].row;
    turn->col = moves[move].col;
}

int ai_turn(GameLoop *gloop, Cell *turn) {
    if (AI_Mode == MODE_EASY) {
        ai_random(gloop->game->map, turn);
    }
    else {
        ai_minimax(gloop->game->map, gloop->game->player, turn, 0, AI_Mode);
    }
    return 0;
}
