#include <stdbool.h>
#include <string.h>

#include <ncurses.h>

#include "tictactoe.h"
#include "ai.h"
#include "tgui.h"


const int sideSize = SIZE * (CELL_SIZE + 1) + 1;


Symbol SCROSS = {{'X', ' ', 'X'},
                 {' ', 'X', ' '},
                 {'X', ' ', 'X'}};

Symbol SZERO = {{'O', 'O', 'O'},
                {'O', ' ', 'O'},
                {'O', 'O', 'O'}};

Symbol SSEL = {{'?', ' ', '?'},
               {' ', '?', ' '},
               {'?', ' ', '?'}};

Symbol SEMPTY = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};




// Menu functions

WINDOW* getMenuWin(int y, int x, int height, int width) {
    WINDOW* menuWin = newwin(height, width, y, x);
    box(menuWin, 0, 0);
    wrefresh(menuWin);
    return menuWin;
}

void drawMenu(WINDOW* menuWin, MenuOption* menuList, int menuLen, int selected) {
    int h, w;
    int y, x;
    int nameL;
    MenuOption* mo;
    getmaxyx(menuWin, h, w);
    y = (h - menuLen) / 2;
    x = w / 2;
    for (int i = 0; i < menuLen; i++) {
        mo = menuList + i;
        nameL = strlen(mo->name);
        if (i == selected)
            wattron(menuWin, A_REVERSE);
        mvwprintw(menuWin, y + i, x - nameL / 2, mo->name);
        if (i == selected)
            wattroff(menuWin, A_REVERSE);
    }
}

MenuOption getMenuOption(int code, char name[256]){
    MenuOption mo;
    mo.code = code;
    strcpy(mo.name, name);
    return mo;
}


// Game map functions

WINDOW* getMapWin(int y, int x) {
    WINDOW* mapWin = newwin(sideSize, sideSize, y, x); 
    wrefresh(mapWin);
    return mapWin;
}

void convertPosition(Cell* position, int* y, int* x) {
    *x = 1 + position->col * (CELL_SIZE + 1);
    *y = 1 + position->row * (CELL_SIZE + 1); 
}

void drawMap(WINDOW* mapWin, int y, int x) {
    for (int j = 0; j < sideSize; j++) { 
        for (int i = 0; i < sideSize; i++) {
            if (j == 0 || j == sideSize - 1) {
                if (i == 0) {
                    mvwaddch(mapWin, y + j, x + i, (j == 0) ? ACS_ULCORNER : ACS_LLCORNER);
                }
                else if (i == sideSize - 1) {
                    mvwaddch(mapWin, y + j, x + i, (j == 0) ? ACS_URCORNER : ACS_LRCORNER);
                }
                else if (i % (CELL_SIZE + 1) == 0) {
                    mvwaddch(mapWin, y + j, x + i, (j == 0) ? ACS_TTEE : ACS_BTEE);
                }
                else {
                    mvwaddch(mapWin, y + j, x + i, ACS_HLINE);
                }
            }
            else if (j % (CELL_SIZE + 1) == 0) {
                if (i == 0 | i == sideSize - 1) {
                    mvwaddch(mapWin, y + j, x + i, (i == 0) ? ACS_LTEE : ACS_RTEE);
                }
                else if (i % (CELL_SIZE + 1) == 0) {
                    mvwaddch(mapWin, y + j, x + i, ACS_PLUS);
                }
                else {
                    mvwaddch(mapWin, y + j, x + i, ACS_HLINE);
                }
            }
            else {
                if (i == 0 || i == sideSize - 1) {
                    mvwaddch(mapWin, y + j, x + i, ACS_VLINE);
                }
                else if (i % (CELL_SIZE + 1) == 0) {
                    mvwaddch(mapWin, y + j, x + i, ACS_VLINE);
                }
            }
        }
    }
}

Symbol* getSymbol(FieldT player) {
    switch (player) {
        case CROSS: return &SCROSS;
        case ZERO: return &SZERO;
        default: return &SEMPTY;
    }
}

void placeSymbol(WINDOW* mapWin, Cell* position, Symbol* symb) {
    int wy, wx;
    convertPosition(position, &wy, &wx);
    for (int i = 0; i < CELL_SIZE; i++) {
        for (int j = 0; j < CELL_SIZE; j++) {
            mvwaddch(mapWin, wy + i, wx + j, (*symb)[i][j]);
        }
    }
}

void drawSymbols(WINDOW* mapWin, FieldT (*map)[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            Symbol* symb = getSymbol((*map)[i][j]);
            Cell pos = Cell_create(i, j);
            placeSymbol(mapWin, &pos, symb);
        }
    }
}


