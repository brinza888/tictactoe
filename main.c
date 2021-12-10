#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ncurses.h>

#include "tictactoe.h"
#include "ai.h"
#include "tgui.h"


InputCode chooseCell(WINDOW* mapWin, FieldT (*map)[SIZE][SIZE], Cell* selection);

char* winnerNames[] = {"Empty", "Cross", "Zero"};


int main(int argc, char** argv) {
    initscr();
    raw();
    noecho();
    keypad(stdscr, true);
    curs_set(false);
    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    
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
    bool menuActive = true;
    int menuL = sizeof(menu_list) / sizeof(MenuOption);
    int menuSel = 0;
    
    // choose ai mode loop
    do {
        wclear(menu_win);
        mvwprintw(menu_win, 1, menu_w / 2 - 7, "Choose AI mode");
        
        switch (ch) {
            case KEY_UP: menuSel--; break;
            case KEY_DOWN: menuSel++; break;
            case '\n':
                menuActive = false;
                break;
        }
        menuSel = (menuSel + menuL) % menuL;  // hold selection in options range
        
        box(menu_win, 0, 0); 
        drawMenu(menu_win, menu_list, menuL, menuSel);
        wrefresh(menu_win);
    } while (menuActive && (ch = getch()) != KEY_F(2) && ch != 'q');
    
    if (ch == KEY_F(2) || ch == 'q') {  // if player pressed F2 or q - exit
        endwin();
        exit(0);
    }

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
    
    // map window
    WINDOW* map_win = getMapWin(3, 5);

    FieldT currentPlayer = CROSS;
    FieldT winner = EMPTY;

    Cell turn;
    
    InputCode inputStatus;
    
    // game loop
    while (1) {
         
        if (currentPlayer == CROSS) {
            inputStatus = chooseCell(map_win, &map, &turn);
            if (inputStatus == NOTONMAP) {  // if player pressed F2 or q - exit
                endwin();
                exit(0);
            }
        }
        else {
            ai_minimax(&map, &turn, currentPlayer, 0);
        }
        
        map[turn.row][turn.col] = currentPlayer;
        
        // draw situation on map
        drawMap(map_win, 0, 0);
        drawSymbols(map_win, &map);
        wrefresh(map_win);

        winner = check_winner(map);  // if win
        if (winner != EMPTY) {
            attron(COLOR_PAIR((winner == CROSS) ? 2 : 3));
            mvprintw(2, 5, "%s is winner here!", winnerNames[winner]);
            attroff(COLOR_PAIR((winner == CROSS) ? 2 : 3));
            refresh();
            break;
        }
        if (is_draw(map)) {  // if draw
            mvprintw(2, 5, "It is draw!");
            refresh();
            break;
        }

        currentPlayer = switch_player(currentPlayer);
    }
    
    mvprintw(0, 0, "To exit press F2 or q buttons!");
     
    while ((ch = getch()) != KEY_F(2) && ch != 'q') {  // just wait for exit key
        ;
    }
    
    endwin();
    return 0;
}


InputCode chooseCell(WINDOW* mapWin, FieldT (*map)[SIZE][SIZE], Cell* selection) {
    int ch;
    InputCode inputStatus = NOTONMAP;
    selection->row = 0;
    selection->col = 0;
    do {
        inputStatus = NOTONMAP;
        switch (ch) {
            case KEY_UP: selection->row--; break;
            case KEY_DOWN: selection->row++; break;
            case KEY_LEFT: selection->col--; break;
            case KEY_RIGHT: selection->col++; break;
            case '\n':
                inputStatus = check_input(*map, *selection);
                break;
        }
        if (inputStatus == INPUT_OK) {
            break;
        }
        else if (inputStatus == NOTEMPTY) {
            attron(COLOR_PAIR(1));
            mvprintw(2, 5, "Cell not empty!");
            refresh();
            attroff(COLOR_PAIR(1));
        }
        else {
            clear();
            refresh();
        }
        
        selection->col = (selection->col + SIZE) % SIZE;  // hold selection in cells range
        selection->row = (selection->row + SIZE) % SIZE;
        
        drawMap(mapWin, 0, 0);
        drawSymbols(mapWin, map);
        wattron(mapWin, COLOR_PAIR(4));
        placeSymbol(mapWin, selection, &SSEL);
        wattroff(mapWin, COLOR_PAIR(4));
        wrefresh(mapWin);
    } while ((ch = getch()) != KEY_F(2) && ch != 'q');

    return inputStatus;
}

