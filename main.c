#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ncurses.h>

#include "tictactoe.h"
#include "ai.h"
#include "game.h"
#include "tgui.h"


#define GM_AI 0
#define GM_TWO 1
#define GM_HOST_NET 2
#define GM_JOIN_NET 3

int game_ai();   // game with AI
int game_two();  // game when two players use one PC


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

    MenuOption gm_menu_opt[] = {
        {GM_AI, "Against AI"},
        {GM_TWO, "Two players"},
        {GM_HOST_NET, "Host network game"},
        {GM_JOIN_NET, "Join network game"},
        {-1, "Exit to shell"}
    };
    Menu *gm_menu = create_menu("Choose game mode", 0, 0, 20, 40, 5, gm_menu_opt);

    int gm_mode;
    bool game_running = true;

    while (game_running) {
        if (run_menu(gm_menu) == -1) {
            break;
        }
        gm_mode = menu_selected(gm_menu);

        switch (gm_mode) {
        case GM_AI:
            game_ai();
            break;
        case GM_TWO:
            game_two();
            break;
        case -1:
            game_running = false;
            break;
        default:
            menu_error(gm_menu, "Feature is not implemented");
            break;
        }
    }

    destroy_menu(gm_menu);
    endwin();

    return 0;   
}

int game_ai() {
    MenuOption ai_menu_opt[] = {
        {MODE_EASY,   "Easy"},
        {MODE_MEDIUM, "Medium"},
        {MODE_HARD,   "Hard"},
        {MODE_EXPERT, "Expert"}
    };

    Menu *ai_mode_menu = create_menu("Choose AI mode", 0, 0, 20, 40, 4, ai_menu_opt);
    if (run_menu(ai_mode_menu) == -1) {
        destroy_menu(ai_mode_menu);
        return 0;
    }
    set_ai_mode(menu_selected(ai_mode_menu));
    destroy_menu(ai_mode_menu);

    Game *game = create_game(CROSS);
    GameLoop *gloop = create_gloop(game, CROSS);
    set_host_turn(gloop, (TurnFunc) keyboard_turn);
    set_othr_turn(gloop, (TurnFunc) ai_turn);

    int result = run_gloop(gloop);

    destroy_gloop(gloop);
    destroy_game(game);

    return result;
}

int game_two() {
    Game *game = create_game(CROSS);
    GameLoop *gloop = create_gloop(game, CROSS);
    set_host_turn(gloop, (TurnFunc) keyboard_turn);
    set_othr_turn(gloop, (TurnFunc) keyboard_turn);

    int result = run_gloop(gloop);

    destroy_gloop(gloop);
    destroy_game(game);

    return result;
}
