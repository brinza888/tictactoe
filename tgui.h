#ifndef TGUI_H
#define TGUI_H

#include <stdbool.h>
#include "tictactoe.h"

#define CELL_SIZE 3


const int sideSize;

// Symbols
typedef char Symbol[CELL_SIZE][CELL_SIZE];

Symbol SCROSS;
Symbol SZERO;
Symbol SSEL;
Symbol SEMPTY;

// For any menus
typedef struct {
    int code;
    char name[256];
} MenuOption;

// Menu functions
MenuOption getMenuOption(int code, char name[256]);

WINDOW* getMenuWin(int y, int x, int height, int width);
void drawMenu(WINDOW* menuWin, MenuOption* menuList, int menuLen, int selected);

// Game map functions
void convertPosition(Cell* position, int* y, int* x);
Symbol* getSymbol(FieldT player);

WINDOW* getMapWin(int y, int x);
void drawMap(WINDOW* mapWin, int y, int x);
void drawSymbols(WINDOW* mapWin, FieldT (*map)[SIZE][SIZE]);
void placeSymbol(WINDOW* mapWin, Cell* position, Symbol* symb);


#endif
