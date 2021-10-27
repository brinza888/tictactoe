#include <stdbool.h>

#include <ncurses.h>

#include "tictactoe.h"
#include "ai.h"


WINDOW* getMapWin(int y, int x, int* height, int* width);


int main(int argc, char** argv) {
    initscr();
    raw();
    noecho();
    keypad(stdscr, true);
    
    int map_h, map_w;
    int scr_h, scr_w;
    int ch;

    getmaxyx(stdscr, scr_h, scr_w);

    //printw("Press f2 to exit");
    refresh();
    
    WINDOW* map_win = getMapWin(3, 3, &map_h, &map_w);

    //refresh();

    while ((ch = getch()) != KEY_F(2)) {
        
        ;
    }

    endwin();
    return 0;
}

WINDOW* getMapWin(int y, int x, int* height, int* width) {
    *height = 2*SIZE + 1;
    *width = 2*SIZE + 1;
    WINDOW* mapWin = newwin(*height, *width, y, x);
    box(mapWin, 0, 0);
    wrefresh(mapWin);
    return mapWin;
}

