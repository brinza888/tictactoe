#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

#include "loopback.h"
#include "utils.h"

static const char SHM_PATH[] = "/tmp/ttt_loopback";
static const int SHM_PROJ = 0x14;

static int shmid;
static GameInfo *game_info;
static GameSlot *slot = NULL;
static bool is_host = false;


int init_loopback() {
    bool init_shmem = false;
    if (access(SHM_PATH, R_OK | W_OK) != 0) {
        creat(SHM_PATH, 0666);
        init_shmem = true;
    }
    key_t shm_key = ftok(SHM_PATH, SHM_PROJ);
    if ((shmid = shmget(shm_key, sizeof(GameInfo), IPC_CREAT | 0666)) == -1) {
        perror("Unable to shmget");
        return -1;
    }
    if ((game_info = shmat(shmid, NULL, 0)) == (void*) -1) {
        perror("Unable to shmat");
        return -1;
    }
    if (init_shmem) {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&game_info->mutex, &attr);
    }
    return 0;
}

void close_loopback() {
    if (is_host && slot != NULL) {
        pthread_mutex_lock(&game_info->mutex);
        slot->state = SS_EMPTY;
        pthread_mutex_unlock(&game_info->mutex);
    }
    shmctl(shmid, IPC_RMID, NULL);
    shmdt(game_info);
}

int host_game() {
    slot = NULL;
    pthread_mutex_lock(&game_info->mutex);
    for (size_t i = 0; i < MAX_SLOTS; i++) {
        if (game_info->slots[i].state == SS_EMPTY) {
            slot = &game_info->slots[i];
            break;
        }
    }
    if (slot == NULL) {
        pthread_mutex_unlock(&game_info->mutex);
        return -1;  // no available slots to host game
    }
    slot->state = SS_WAIT;
    slot->turn_done = false;
    slot->move_maker = CROSS;
    is_host = true;
    pthread_mutex_unlock(&game_info->mutex);
    return 0;
}

int join_game() {
    pthread_mutex_lock(&game_info->mutex);
    for (size_t i = 0; i < MAX_SLOTS; i++) {
        if (game_info->slots[i].state == SS_WAIT) {
            slot = &game_info->slots[i];
            break;
        }
    }
    if (slot == NULL) {
        pthread_mutex_unlock(&game_info->mutex);
        return -1;  // no slots to join
    }
    slot->state = SS_GAME;
    is_host = false;
    pthread_mutex_unlock(&game_info->mutex);
    return 0;
}

int loopback_make_turn(GameLoop *gloop, Cell *turn) {
    int res = keyboard_turn(gloop, turn);
    if (res == 0) {
        pthread_mutex_lock(&game_info->mutex);
        memcpy(&slot->turn, turn, sizeof(Cell));
        slot->turn_done = true;
        pthread_mutex_unlock(&game_info->mutex);
    }
    return res;
}

int loopback_wait_turn(GameLoop *gloop, Cell *turn) {
    int res;
    pthread_mutex_lock(&game_info->mutex);
    if (slot->turn_done && slot->move_maker != gloop->host_player) {
        memcpy(turn, &slot->turn, sizeof(Cell));
        slot->turn_done = false;
        slot->move_maker = gloop->host_player;
        res = 0;
    }
    else {
        res = 1;
    }
    pthread_mutex_unlock(&game_info->mutex);
    return res;
}
