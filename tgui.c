#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <ncurses.h>

#include "tictactoe.h"
#include "ai.h"
#include "tgui.h"


const int sideSize = SIZE * (CELL_SIZE + 1) + 1;

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
    {'X', ' ', '\0', ' ', 'X'},
    {' ', '\0', '\0', '\0', ' '},
    {' ', '\0', '\0', '\0', '\0'},
    {' ', '\0', '\0', '\0', ' '},
    {'X', ' ', '\0', ' ', 'X'}};

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
    menu->active = true;
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
    mvwprintw(menu->window, 1, w / 2 - 7, "%s", menu->title);
    box(menu->window, 0, 0);

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
    } while (menu->active && (ch = getch()) != KEY_F(2) && ch != 'q');
    
    // if player pressed F2 or q - return negative
    if (ch == KEY_F(2) || ch == 'q') {
        return -1;
    }

    return menu->selected;
}

int get_selected(Menu *menu) {
    return menu->opt[menu->selected].code;
}

void destroy_menu(Menu *menu) {
    delwin(menu->window);
    free(menu);
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
                if ((i == 0) | (i == sideSize - 1)) {
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
                if ((i == 0) || (i == sideSize - 1)) {
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
            if ((*symb)[i][j] == '\0') {
                continue;
            }
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


