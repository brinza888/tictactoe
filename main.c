#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ncurses.h>

#include "tictactoe.h"
#include "ai.h"
#include "tgui.h"


int main(int argc, char* argv[]) {
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
    
    refresh();
    
    MenuOption ai_menu_opt[] = {
        {MODE_EASY,   "Easy"},
        {MODE_MEDIUM, "Medium"},
        {MODE_HARD,   "Hard"},
        {MODE_EXPERT, "Expert"}
    };

    Menu *ai_mode_menu = create_menu("Choose AI mode",
                                     0, 0, 20, 40,
                                     4, ai_menu_opt);

    if (run_menu(ai_mode_menu) == -1) {
        destroy_menu(ai_mode_menu);
        endwin();
        return 0;
    }

    int ai_mode = menu_selected(ai_mode_menu);

    destroy_menu(ai_mode_menu);
    clear();
    refresh();
    timeout(0);

    Game *game = create_game(CROSS);
    WINDOW* game_win = game_window(3, 5);

    bool running = true;
    Cell sel = {0, 0};  // selection
    int ch = 0;
    bool do_make_turn = false;
    
    // game loop
    while (running) {
        ch = getch();

        switch (ch) {
            case KEY_UP:    sel.row = (sel.row - 1 + MAP_SIZE) % MAP_SIZE; break;
            case KEY_DOWN:  sel.row = (sel.row + 1) % MAP_SIZE;            break;
            case KEY_LEFT:  sel.col = (sel.col - 1 + MAP_SIZE) % MAP_SIZE; break;
            case KEY_RIGHT: sel.col = (sel.col + 1) % MAP_SIZE;            break;
            case '\n':      do_make_turn = true;                           break;
        }

        if (game->player == CROSS && do_make_turn) {
            do_make_turn = false;
            if (make_turn(game, sel) == E_NOTEMPTY) {
                attron(COLOR_PAIR(1));
                mvprintw(2, 5, "Cell not empty!");
                refresh();
                attroff(COLOR_PAIR(1));
            }
        }
        
        if (game->player == ZERO) {
            ai_make_turn(game, ai_mode);
        }

        finalize_turn(game);

        draw_map(game_win, 0, 0);
        draw_symbols(game_win, game->map);

        wattron(game_win, COLOR_PAIR(4));
        place_symbol(game_win, sel, &SSEL);
        wattroff(game_win, COLOR_PAIR(4));

        wrefresh(game_win);

        if (game->winner != EMPTY) {
            attron(COLOR_PAIR((game->winner == CROSS) ? 2 : 3));
            mvprintw(2, 5, "%s is the winner here!", player_name(game->winner));
            attroff(COLOR_PAIR((game->winner == CROSS) ? 2 : 3));
            refresh();
            running = false;
        }

        if (game->is_draw) {
            mvprintw(2, 5, "It is draw!");
            refresh();
            running = false;
        }

        if (ch == KEY_F(2) || ch == 'q')
            running = false;
    }
    
    mvprintw(0, 0, "End of game! To exit press: F2 or Q");
    while ((ch = getch()) != KEY_F(2) && ch != 'q');  // wait user for exit
    
    destroy_game(game);
    delwin(game_win);
    endwin();

    return 0;
}
