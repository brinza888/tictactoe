#ifndef TGUI_H
#define TGUI_H

#include <stdbool.h>

#include "tictactoe.h"
#include "utils.h"

#define CELL_SIZE 5
#define MAX_MENU_OPTION_NAME 32
#define MAX_MENU_TITLE 128

extern const int sideSize;

// Symbols
typedef char Symbol[CELL_SIZE][CELL_SIZE];

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

    int opt_count;
    MenuOption *opt;  // pointer to options array
    
    Selection *sel;
} Menu;

// Menu functions
Menu *create_menu(const char *title, int x, int y, int h, int w, int opt_count, MenuOption* opt);
void draw_menu(const Menu *menu);
int run_menu(Menu *menu);
int menu_selected(Menu *menu);
void menu_error(Menu *menu, const char *error);
void destroy_menu(Menu *menu);

// Game map functions
void convert_position(Cell position, int *y, int *x);
Symbol* get_symbol(Player player);

WINDOW* game_window(int y, int x);
void draw_grid(WINDOW* game_win, int y, int x);
void draw_symbol(WINDOW* game_win, Cell pos, Symbol* symb, int dy, int dx);
void draw_map(WINDOW* game_win, Map map, int dy, int dx);
void draw_sel(WINDOW* game_win, Cell pos, int dy, int dx);


#endif
