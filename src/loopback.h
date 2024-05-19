#ifndef __LOOPBACK_H
#define __LOOPBACK_H

#include "tictactoe.h"
#include "game.h"

#define MAX_SLOTS 64
#define MAX_PATH 256

typedef enum _SlotState {
    SS_EMPTY,
    SS_WAIT,
    SS_GAME,
} SlotState;

typedef struct _GameSlot {
    SlotState state;
    Player move_maker;
    Cell turn;
    bool turn_done;
} GameSlot;

typedef struct _GameInfo {
    pthread_mutex_t mutex;
    GameSlot slots[MAX_SLOTS];
} GameInfo;

int init_loopback();
void close_loopback();

int host_game();
int join_game();

int loopback_make_turn(GameLoop *gloop, Cell *turn);
int loopback_wait_turn(GameLoop *gloop, Cell *turn);

#define LB_MAKE_TURN ((TurnFunc) loopback_make_turn)
#define LB_WAIT_TURN ((TurnFunc) loopback_wait_turn)

#endif
