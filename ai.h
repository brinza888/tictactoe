#ifndef AI_H
#define AI_H

#include "tictactoe.h"

#define MODE_EASY 0
#define MODE_MEDIUM 1
#define MODE_HARD 2
#define MODE_EXPERT -1

int get_moves(const Map map, Cell *moves);

int ai_minimax(Map map, Player player, Cell *turn, int depth, int depthMax);
void ai_random(Map map, Cell* turn);

void ai_make_turn(Game *game, int ai_mode);

#endif

