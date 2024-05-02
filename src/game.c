#include <stdlib.h>

#include <ncurses.h>

#include "game.h"
#include "tgui.h"


GameLoop *create_gloop(Game *game, Player host_player) {
    GameLoop *gloop = calloc(sizeof(GameLoop), 1);
    gloop->window = game_window(3, 5);
    gloop->running = false;
    gloop->host_player = host_player;
    gloop->othr_player = switch_player(host_player);
    gloop->game = game;
    gloop->row = create_sel(0, 0, MAP_SIZE - 1, false);
    gloop->col = create_sel(0, 0, MAP_SIZE - 1, false);
    gloop->enter_hit = false;
    gloop->host_turn = NULL;
    gloop->othr_turn = NULL;
    return gloop;
}

void set_host_turn(GameLoop *gloop, TurnFunc host_turn) {
    gloop->host_turn = host_turn;
}

void set_othr_turn(GameLoop *gloop, TurnFunc othr_turn) {
    gloop->othr_turn = othr_turn;
}

int keyboard_turn(GameLoop *gloop, Cell *turn) {
    if (gloop->enter_hit) {
        turn->row = sel_val(gloop->row);
        turn->col = sel_val(gloop->col);
        return 0;
    }
    return 1;
}

int run_gloop(GameLoop *gloop) {
    int ch = 0;
    Game *game = gloop->game;
    Cell sel = {0, 0};
    Cell turn = {0, 0};

    gloop->running = true;
    bool turn_done = false;

    clear();
    refresh();
    flushinp();
    timeout(1);

    int dy = 0;
    int dx = 0;

    while (gloop->running) {
        gloop->enter_hit = false;
        turn_done = false;
        
        ch = getch();
        switch (ch) {
            case KEY_UP:    sel_prev(gloop->row);   break;
            case KEY_DOWN:  sel_next(gloop->row);   break;
            case KEY_LEFT:  sel_prev(gloop->col);   break;
            case KEY_RIGHT: sel_next(gloop->col);   break;
            case KEY_F(2):  gloop->running = false; break;
            case 'q':       gloop->running = false; break;
            case '\n':      gloop->enter_hit = true;    break;
        }
        
        if (ch != ERR) {
            sel.row = sel_val(gloop->row);
            sel.col = sel_val(gloop->col);
            // dy = -sel.row * CELL_SIZE;
            // dx = -sel.col * CELL_SIZE;
            wclear(gloop->window);
        }

        if (game->player == gloop->host_player) {
            if (gloop->host_turn(gloop, &turn) == 0) {
                turn_done = true;
            }
        }

        if (game->player == gloop->othr_player) {
            if (gloop->othr_turn(gloop, &turn) == 0) {
                turn_done = true;
            }
        }

        if (turn_done) {
            if (make_turn(game, turn) == E_NOTEMPTY) {
                attron(COLOR_PAIR(1));
                mvprintw(2, 5, "Cell not empty!");
                attroff(COLOR_PAIR(1));
                refresh();
            }
            else {
                finalize_turn(game);
            }
        }

        draw_grid(gloop->window, dy, dx);
        draw_map(gloop->window, game->map, dy, dx);
        draw_sel(gloop->window, sel, dy, dx);
        wrefresh(gloop->window);

        if (game->winner != EMPTY) {
            attron(COLOR_PAIR((game->winner == CROSS) ? 2 : 3));
            mvprintw(2, 5, "%s is the winner here!", player_name(game->winner));
            attroff(COLOR_PAIR((game->winner == CROSS) ? 2 : 3));
            refresh();
            gloop->running = false;
        }
        else if (game->is_draw) {
            mvprintw(2, 5, "It is draw!");
            refresh();
            gloop->running = false;
        }
    }

    mvprintw(0, 0, "Game end! To exit press F2 or q");
    refresh();
    timeout(-1);
    
    while ((ch = getch()) != KEY_F(2) && ch != 'q');

    return 0;
}

void destroy_gloop(GameLoop *gloop) {
    destroy_sel(gloop->row);
    destroy_sel(gloop->col);
    delwin(gloop->window);
    free(gloop);
}
