#include <stdio.h>
#include <stdbool.h>

#include <ncurses.h>

#include "tictactoe.h"
#include "ai.h"
#include "tgui.h"


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

