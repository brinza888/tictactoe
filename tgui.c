#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <ncurses.h>

#include "tictactoe.h"
#include "ai.h"
#include "tgui.h"


const int sideSize = MAP_SIZE * (CELL_SIZE + 1) + 1;

Symbol SCROSS = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', 'X', ' ', 'X', ' '},
    {' ', ' ', 'X', ' ', ' '},
    {' ', 'X', ' ', 'X', ' '},
    {' ', ' ', ' ', ' ', ' '}};

Symbol SZERO = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', 'O', 'O', 'O', ' '},
    {' ', 'O', ' ', 'O', ' '},
    {' ', 'O', 'O', 'O', ' '},
    {' ', ' ', ' ', ' ', ' '}};

Symbol SSEL = {
    {'X', '\0', '\0', '\0', 'X'},
    {'\0', '\0', '\0', '\0', '\0'},
    {'\0', '\0', '\0', '\0', '\0'},
    {'\0', '\0', '\0', '\0', '\0'},
    {'X', '\0', '\0', '\0', 'X'}};

Symbol SEMPTY = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}};

// Menu functions

Menu *create_menu(const char *title, int x, int y, int h, int w, size_t opt_count, MenuOption* opt) {
    Menu *menu = calloc(sizeof(Menu), 1);
    menu->window = newwin(h, w, y, x);
    box(menu->window, 0, 0);
    wrefresh(menu->window);
    menu->opt_count = opt_count;
    menu->opt = opt;
    menu->selected = 0;
    menu->active = false;
    menu->error = false;
    strcpy(menu->title, title);
    return menu;
}

void draw_menu(const Menu *menu) {
    MenuOption *opt;
    size_t opt_name_len;

    int h, w;
    getmaxyx(menu->window, h, w);
    int y = (h - menu->opt_count) / 2;
    int x = w / 2;

    wclear(menu->window);
    mvwprintw(menu->window, 1, (w - strlen(menu->title)) / 2, "%s", menu->title);
    box(menu->window, 0, 0);

    if (menu->error) {
        wattron(menu->window, COLOR_PAIR(1));
        mvwprintw(menu->window, 3, (w - strlen(menu->last_error)) / 2, "%s", menu->last_error);
        wattroff(menu->window, COLOR_PAIR(1));
    }

    for (size_t i = 0; i < menu->opt_count; i++) {
        opt = menu->opt + i;
        opt_name_len = strlen(opt->name);
        if (i == menu->selected)
            wattron(menu->window, A_REVERSE);
        mvwprintw(menu->window, y + i, x - opt_name_len / 2, "%s", opt->name);
        if (i == menu->selected)
            wattroff(menu->window, A_REVERSE);
    }
}

int run_menu(Menu *menu) {
    clear();
    refresh();
    menu->active = true;
    int ch;
    do {
        // switch selection
        switch (ch) {
            case KEY_UP: menu->selected--; break;
            case KEY_DOWN: menu->selected++; break;
            case '\n':
                menu->active = false;
                break;
        }
        // hold selection in options range
        menu->selected = (menu->selected + menu->opt_count) % menu->opt_count;
        // draw menu and refresh
        draw_menu(menu);
        wrefresh(menu->window);
        menu->error = false;
    } while (menu->active && (ch = getch()) != KEY_F(2) && ch != 'q');
    
    // if player pressed F2 or q - return negative
    if (ch == KEY_F(2) || ch == 'q') {
        return -1;
    }

    return menu->selected;
}

int menu_selected(Menu *menu) {
    return menu->opt[menu->selected].code;
}

void menu_error(Menu *menu, const char *error) {
    strncpy(menu->last_error, error, MAX_MENU_TITLE);
    menu->last_error[MAX_MENU_TITLE - 1] = '\0';
    menu->error = true;
}

void destroy_menu(Menu *menu) {
    delwin(menu->window);
    free(menu);
}


// Game map functions

WINDOW* game_window(int y, int x) {
    WINDOW* game_win = newwin(sideSize, sideSize, y, x); 
    wrefresh(game_win);
    return game_win;
}

void convert_position(Cell pos, int *y, int *x) {
    *x = 1 + pos.col * (CELL_SIZE + 1);
    *y = 1 + pos.row * (CELL_SIZE + 1); 
}

void draw_map(WINDOW* game_win, int y, int x) {
    for (int j = 0; j < sideSize; j++) { 
        for (int i = 0; i < sideSize; i++) {
            if (j == 0 || j == sideSize - 1) {
                if (i == 0) {
                    mvwaddch(game_win, y + j, x + i, (j == 0) ? ACS_ULCORNER : ACS_LLCORNER);
                }
                else if (i == sideSize - 1) {
                    mvwaddch(game_win, y + j, x + i, (j == 0) ? ACS_URCORNER : ACS_LRCORNER);
                }
                else if (i % (CELL_SIZE + 1) == 0) {
                    mvwaddch(game_win, y + j, x + i, (j == 0) ? ACS_TTEE : ACS_BTEE);
                }
                else {
                    mvwaddch(game_win, y + j, x + i, ACS_HLINE);
                }
            }
            else if (j % (CELL_SIZE + 1) == 0) {
                if ((i == 0) | (i == sideSize - 1)) {
                    mvwaddch(game_win, y + j, x + i, (i == 0) ? ACS_LTEE : ACS_RTEE);
                }
                else if (i % (CELL_SIZE + 1) == 0) {
                    mvwaddch(game_win, y + j, x + i, ACS_PLUS);
                }
                else {
                    mvwaddch(game_win, y + j, x + i, ACS_HLINE);
                }
            }
            else {
                if ((i == 0) || (i == sideSize - 1)) {
                    mvwaddch(game_win, y + j, x + i, ACS_VLINE);
                }
                else if (i % (CELL_SIZE + 1) == 0) {
                    mvwaddch(game_win, y + j, x + i, ACS_VLINE);
                }
            }
        }
    }
}

Symbol* get_symbol(Player player) {
    switch (player) {
        case CROSS: return &SCROSS;
        case ZERO: return &SZERO;
        default: return &SEMPTY;
    }
}

void place_symbol(WINDOW* game_win, Cell pos, Symbol* symb) {
    int y, x;
    convert_position(pos, &y, &x);
    for (int i = 0; i < CELL_SIZE; i++) {
        for (int j = 0; j < CELL_SIZE; j++) {
            if ((*symb)[i][j] == '\0') {
                continue;
            }
            mvwaddch(game_win, y + i, x + j, (*symb)[i][j]);
        }
    }
}

void draw_symbols(WINDOW* game_win, Map map) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            Symbol* symb = get_symbol(map[i][j]);
            place_symbol(game_win, cell(i, j), symb);
        }
    }
}


