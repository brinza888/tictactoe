#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ncurses.h>

#include "tictactoe.h"
#include "ai.h"
#include "game.h"
#include "tgui.h"
#include "lobby.h"


#define GM_AI 0
#define GM_TWO 1
#define GM_HOST_NET 2
#define GM_JOIN_NET 3

int game_ai();   // game with AI
int game_two();  // game when two players use one PC
int game_host_net();
int game_join_net();
Player choose_player(const char *title);


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
        {GM_HOST_NET, "Host game"},
        {GM_JOIN_NET, "Join game"},
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
        case GM_HOST_NET:
            game_host_net();
            break;
        case GM_JOIN_NET:
            game_join_net();
            break;
        case -1:
            game_running = false;
            break;
        default:
            menu_error(gm_menu, "Feature is not implemented or restricted");
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

    Player host_player, first_player; 
    if ((host_player = choose_player("Choose host player")) == EMPTY) return -1;
    if ((first_player = choose_player("Who makes turn first?")) == EMPTY) return -1;

    Menu *ai_mode_menu = create_menu("Choose AI mode", 0, 0, 20, 40, 4, ai_menu_opt);
    if (run_menu(ai_mode_menu) == -1) {
        destroy_menu(ai_mode_menu);
        return 0;
    }
    set_ai_mode(menu_selected(ai_mode_menu));
    destroy_menu(ai_mode_menu);

    Game *game = create_game(first_player);
    GameLoop *gloop = create_gloop(game, host_player);

    set_host_turn(gloop, KEYBOARD_TURN);
    set_othr_turn(gloop, AI_TURN);

    int result = run_gloop(gloop);

    destroy_gloop(gloop);
    destroy_game(game);

    return result;
}

int game_two() {
    Player first_player;
    if ((first_player = choose_player("Who makes turn first?")) == EMPTY) return -1;

    Game *game = create_game(first_player);
    GameLoop *gloop = create_gloop(game, first_player);
    set_host_turn(gloop, KEYBOARD_TURN);
    set_othr_turn(gloop, KEYBOARD_TURN);

    int result = run_gloop(gloop);

    destroy_gloop(gloop);
    destroy_game(game);

    return result;
}

int game_host_net() {
    init_loopback();
    host_game();

    Game *game = create_game(CROSS);

    GameLoop *gloop = create_gloop(game, CROSS);
    set_host_turn(gloop, LB_TURN);
    set_othr_turn(gloop, LB_OTHER_TURN);

    int result = run_gloop(gloop);

    close_loopback();

    return result;
}

int game_join_net() {
    init_loopback();
    join_game();

    Game *game = create_game(CROSS);

    GameLoop *gloop = create_gloop(game, ZERO);
    set_host_turn(gloop, LB_TURN);
    set_othr_turn(gloop, LB_OTHER_TURN);

    int result = run_gloop(gloop);

    close_loopback();

    return result;
}

Player choose_player(const char *title) {
    MenuOption player_opt[] = {
        {CROSS, "Cross (X)"},
        {ZERO, "Zero (O)"}
    };
    Menu *player_menu = create_menu(title, 0, 0, 20, 40, 2, player_opt);
    if (run_menu(player_menu) == -1) {
        return EMPTY;
    }
    Player choosen = (Player) menu_selected(player_menu);
    destroy_menu(player_menu);
    return choosen;
}
