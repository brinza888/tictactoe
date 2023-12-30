#include <stdlib.h>

#include <ncurses.h>

#include "game.h"
#include "tgui.h"


GameLoop *create_gloop(Game *game, Player host_player) {
    GameLoop *gloop = calloc(sizeof(GameLoop), 1);
    gloop->window = game_window(3, 5);
    gloop->running = false;
    gloop->host_player = host_player;
    gloop->game = game;
    gloop->host_turn = NULL;
    gloop->othr_turn = NULL;
    return gloop;
}

void set_host_turn(GameLoop *gloop, FHostTurn host_turn) {
    gloop->host_turn = host_turn;
}

void set_othr_turn(GameLoop *gloop, FOthrTurn othr_turn) {
    gloop->othr_turn = othr_turn;
}

int run_gloop(GameLoop *gloop) {
    int ch = 0;
    bool do_make_turn = false;
    Cell sel = {0, 0};
    Game *game = gloop->game;

    gloop->running = true;

    clear();
    refresh();
    timeout(0);
    
    while (gloop->running) {
        ch = getch();
        switch (ch) {
            case KEY_UP:    sel.row = (sel.row - 1 + MAP_SIZE) % MAP_SIZE; break;
            case KEY_DOWN:  sel.row = (sel.row + 1) % MAP_SIZE;            break;
            case KEY_LEFT:  sel.col = (sel.col - 1 + MAP_SIZE) % MAP_SIZE; break;
            case KEY_RIGHT: sel.col = (sel.col + 1) % MAP_SIZE;            break;
            case KEY_F(2):  gloop->running = false;                        break;
            case 'q':       gloop->running = false;                        break;
            case '\n':      do_make_turn = true;                           break;
        }

        if (game->player == gloop->host_player && do_make_turn) {
            do_make_turn = false;
            if (gloop->host_turn(game, sel) == E_NOTEMPTY) {
                attron(COLOR_PAIR(1));
                mvprintw(2, 5, "Cell not empty!");
                attroff(COLOR_PAIR(1));
                refresh();
            }
        }
        if (game->player == ZERO) {
            Cell turn = gloop->othr_turn(game);  // wait other player for make turn
            if (turn.row != -1 && turn.col != -1) {
                make_turn(game, turn);
            }
        }

        finalize_turn(game);

        draw_map(gloop->window, 0, 0);
        draw_symbols(gloop->window, game->map);

        wattron(gloop->window, COLOR_PAIR(4));
        place_symbol(gloop->window, sel, &SSEL);
        wattroff(gloop->window, COLOR_PAIR(4));

        wrefresh(gloop->window);

        if (game->winner != EMPTY) {
            attron(COLOR_PAIR((game->winner == CROSS) ? 2 : 3));
            mvprintw(2, 5, "%s is the winner here!", player_name(game->winner));
            attroff(COLOR_PAIR((game->winner == CROSS) ? 2 : 3));
            refresh();
            gloop->running = false;
        }
        if (game->is_draw) {
            mvprintw(2, 5, "It is draw!");
            refresh();
            gloop->running = false;
        }
    }
    timeout(-1);

    while ((ch = getch()) != KEY_F(2) && ch != 'q');

    return 0;
}

void destroy_gloop(GameLoop *gloop) {
    delwin(gloop->window);
    free(gloop);
}
