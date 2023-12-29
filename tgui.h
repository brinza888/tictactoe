#ifndef TGUI_H
#define TGUI_H

#include <stdbool.h>
#include "tictactoe.h"

#define CELL_SIZE 5
#define MAX_MENU_OPTION_NAME 32
#define MAX_MENU_TITLE 128

extern const int sideSize;

// Symbols
typedef char Symbol[CELL_SIZE][CELL_SIZE];

extern Symbol SCROSS;
extern Symbol SZERO;
extern Symbol SSEL;
extern Symbol SEMPTY;

// For any menus
typedef struct _MenuOption {
    int code;
    char name[MAX_MENU_OPTION_NAME];
} MenuOption;

typedef struct _Menu {
    char title[MAX_MENU_TITLE];
    WINDOW* window;
    bool active;

    size_t opt_count;
    MenuOption *opt;  // pointer to options array
    size_t selected;
} Menu;

// Menu functions
Menu *create_menu(const char *title, int x, int y, int h, int w, size_t opt_count, MenuOption* opt);
void draw_menu(const Menu *menu);
int run_menu(Menu *menu);
int menu_selected(Menu *menu);
void destroy_menu(Menu *menu);

// Game map functions
void convertPosition(Cell* position, int* y, int* x);
Symbol* getSymbol(FieldT player);

WINDOW* getMapWin(int y, int x);
void drawMap(WINDOW* mapWin, int y, int x);
void drawSymbols(WINDOW* mapWin, FieldT (*map)[SIZE][SIZE]);
void placeSymbol(WINDOW* mapWin, Cell* position, Symbol* symb);


#endif
