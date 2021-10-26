#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "tictactoe.h"
#include "minimax.h"


int main(int argc, char** argv){
    enum field map[SIZE][SIZE];
    
    srand(time(NULL));
    
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            map[i][j] = EMPTY;  // fill map with empty cells
        }
    }

    char current_ch;  // current player char
    printf("Who is the first? [X/O]: ");
    scanf("%c", &current_ch);
    FieldT current = char2field(current_ch);

    char ai_mode_ch;
    int ai_mode;
    printf("AI mode?\n");
    printf("Easy Medium Hard Expert [E/M/H/X]: ");
    scanf("%*c%c", &ai_mode_ch);
    switch (ai_mode_ch) {
        case 'E': ai_mode = MODE_EASY; break;
        case 'M': ai_mode = MODE_MEDIUM; break;
        case 'H': ai_mode = MODE_HARD; break;
        case 'X': ai_mode = MODE_EXPERT; break;
        default:
            printf("Incorrect mode specified!\n");
            return 1;
            break;
    }

    setMode(ai_mode);
    
    if (current == EMPTY) {
        printf("You may choose only X or O\n");
        return 1;
    }
    
    int turns_count = 1;

    Cell turn = Cell_create(-1, -1); // turn cell
    int ipt_r;  // result of cell input

    FieldT winner = EMPTY;
    bool is_running = true;

    while (is_running) {
        system("clear");  // clear last output

        current_ch = field2char(current);  // get char representation of player
        
        printf("===== %d TURN (%c) =====\n\n", turns_count, current_ch);
        print_map(map);
        printf("\n");
 
        if (current == CROSS) {  // Human turn
            ipt_r = INPUT_OK;
            do {
                switch (ipt_r) {
                    case NOTONMAP: printf("Not on map!\n"); break;
                    case NOTEMPTY: printf("Already taken!\n"); break;
                }
                printf("Choose row and column to make turn:\n");
                scanf("%d %d", &turn.row, &turn.col);
                turn.row--;
                turn.col--;
            } while ((ipt_r = check_input(map, turn)) != INPUT_OK);
        }
        else if (current == ZERO) {  // AI turn
            ai_minimax(&map, &turn, current, 0);
            // ai_random(&map, &turn);
        }
        
        map[turn.row][turn.col] = current;  // fill cell with player's symbol        
        
        winner = check_winner(map);  // check winner
        
        if (winner != EMPTY) {  // if winner exists
            system("clear");
            printf("===== WIN =====\n\n");
            
            print_map(map);
            printf("\n");

            printf("%c is winner here! \n", current_ch);
            printf("%d moves was enough for him!\n\n", turns_count);
            break;
        }
        if (is_draw(map)) {  // check if draw
            system("clear");
            printf("===== DRAW =====\n\n");
            
            print_map(map);
            printf("\n");
            
            printf("There are no empty cells left after %d turns!\n", turns_count);
            printf("This is a draw! Winners all around =)\n\n");
            break;
        }

        turns_count++;  // turns counting
        current = switch_player(current);  // player switching
    }
    
	return 0;
}

