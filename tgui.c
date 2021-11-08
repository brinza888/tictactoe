#include <stdbool.h>

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
    
    int map_h, map_w;
    int scr_h, scr_w;
    int ch = 0;
    
    Cell selection;
    selection.row = 0;
    selection.col = 0;
    
    getmaxyx(stdscr, scr_h, scr_w);
    refresh();
    
    WINDOW* map_win = getMapWin(3, 3, &map_h, &map_w);
    
    do {
        wclear(map_win);
        mvprintw(0, 0, "To exit press q or F2");
        
        switch (ch) {
            case KEY_UP: selection.row = (SIZE + selection.row - 1) % SIZE; break;
            case KEY_DOWN: selection.row = (SIZE + selection.row + 1) % SIZE; break;
            case KEY_LEFT: selection.col = (SIZE + selection.col - 1) % SIZE; break;
            case KEY_RIGHT: selection.col = (SIZE + selection.col + 1) % SIZE; break;
        }
        
        drawMap(map_win, 0, 0);
        placeSymbol(map_win, &selection, &SSEL);  
        wrefresh(map_win);
    } while ((ch = getch()) != KEY_F(2) && ch != 'q');

    endwin();
    return 0;
}

WINDOW* getMapWin(int y, int x, int* height, int* width) {
    *height = sideSize;
    *width = sideSize;
    WINDOW* mapWin = newwin(*height, *width, y, x); 
    wrefresh(mapWin);
    return mapWin;
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

void convertPosition(Cell* position, int* y, int* x) {
    *x = 1 + position->col * (CELL_SIZE + 1);
    *y = 1 + position->row * (CELL_SIZE + 1); 
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
