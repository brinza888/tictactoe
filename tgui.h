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

    bool error;
    char last_error[MAX_MENU_TITLE];

    size_t opt_count;
    MenuOption *opt;  // pointer to options array
    size_t selected;
} Menu;

// Menu functions
Menu *create_menu(const char *title, int x, int y, int h, int w, size_t opt_count, MenuOption* opt);
void draw_menu(const Menu *menu);
int run_menu(Menu *menu);
int menu_selected(Menu *menu);
void menu_error(Menu *menu, const char *error);
void destroy_menu(Menu *menu);

// Game map functions
void convert_position(Cell position, int *y, int *x);
Symbol* get_symbol(Player player);

WINDOW* game_window(int y, int x);
void draw_map(WINDOW* game_win, int y, int x);
void place_symbol(WINDOW* game_win, Cell pos, Symbol* symb);
void draw_symbols(WINDOW* game_win, Map map);


#endif
