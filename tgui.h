#ifndef TGUI_H
#define TGUI_H

#include "tictactoe.h"


#define CELL_SIZE 3

const int sideSize;


typedef char Symbol[CELL_SIZE][CELL_SIZE];

void convertPosition(Cell* position, int* y, int* x);
Symbol* getSymbol(FieldT player);


WINDOW* getMapWin(int y, int x, int* height, int* width);
void drawMap(WINDOW* mapWin, int y, int x);
void placeSymbol(WINDOW* mapWin, Cell* position, Symbol* symb);

#endif
