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


int main(int argc, char** argv) {
    initscr();
    raw();
    noecho();
    keypad(stdscr, true);
    curs_set(false);
    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    
    int scr_h, scr_w;
    int ch = 0;
    
    getmaxyx(stdscr, scr_h, scr_w);
    refresh();
    
    int menu_h = 20;
    int menu_w = 40;
    
    WINDOW* menu_win = getMenuWin(3, 5, menu_h, menu_w);
    MenuOption menu_list[] = {
        getMenuOption(MODE_EASY, "Easy"),
        getMenuOption(MODE_MEDIUM, "Medium"),
        getMenuOption(MODE_HARD, "Hard"),
        getMenuOption(MODE_EXPERT, "Expert")
    };
    int menuL = sizeof(menu_list) / sizeof(MenuOption);
    int menuSel = 0;
    
    // choose ai mode loop
    do {
        wclear(menu_win);
        mvwprintw(menu_win, 1, menu_w / 2 - 7, "Choose AI mode");
        
        switch (ch) {
            case KEY_UP: menuSel--; break;
            case KEY_DOWN: menuSel++; break;
        }
        menuSel = (menuSel + menuL) % menuL;  // hold selection in options range
        
        box(menu_win, 0, 0); 
        drawMenu(menu_win, menu_list, menuL, menuSel);
        wrefresh(menu_win);
    } while ((ch = getch()) != KEY_F(2) && ch != 'q');

    int ai_mode = menu_list[menuSel].code;

    setMode(ai_mode);
    
    clear();
    refresh();

    // internal map
    FieldT map[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            map[i][j] = EMPTY;
        }
    }
    map[1][1] = CROSS;
    
    // map window
    WINDOW* map_win = getMapWin(3, 5);
    Cell mapSel;
    mapSel.row = 0;
    mapSel.col = 0;

    int input_status = NOTONMAP;
    
    // choose cell loop
    while (1) {
        do {
            wclear(map_win);
            
            switch (ch) {
                case KEY_UP: mapSel.row--; break;
                case KEY_DOWN: mapSel.row++; break;
                case KEY_LEFT: mapSel.col--; break;
                case KEY_RIGHT: mapSel.col++; break;
                case KEY_ENTER:
                    input_status = check_input(map, mapSel);
                    if (input_status == NOTEMPTY) {
                        attron(COLOR_PAIR(1));
                        mvprintw(0, 0, "Cell not empty!");
                        attroff(COLOR_PAIR(1));
                    }
                    break;
            }
            if (input_status == INPUT_OK) {
                break;
            }
            
            mapSel.col = (mapSel.col + SIZE) % SIZE;  // hold selection in cells range
            mapSel.row = (mapSel.row + SIZE) % SIZE;
            
            drawMap(map_win, 0, 0);
            drawSymbols(map_win, &map);
            placeSymbol(map_win, &mapSel, &SSEL);  
            wrefresh(map_win);
        } while ((ch = getch()) != KEY_F(2) && ch != 'q');
    }

    endwin();
    return 0;
}


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


