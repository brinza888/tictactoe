#ifndef AI_H
#define AI_H

#include "tictactoe.h"

#define MODE_EASY 0
#define MODE_MEDIUM 1
#define MODE_HARD 2
#define MODE_EXPERT -1

int get_moves(const Map map, Cell *moves);

void set_ai_mode(int ai_mode);
int get_ai_mode();

int ai_minimax(Map map, Player player, Cell *turn, int depth, int depthMax);
void ai_random(Map map, Cell* turn);

Cell ai_turn(Game *game);

#endif

