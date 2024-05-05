#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <signal.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#include "loopback.h"
#include "utils.h"

#define lock_sem(sem) semop((sem), &(struct sembuf){0, 1, 0}, 1);
#define release_sem(sem) semop((sem), &(struct sembuf){0, -1, 0}, 1);
#define wait_sem(sem) semop((sem), &(struct sembuf){0, 0, 0}, 1);

static const char SHM_PATH[] = "/tmp/ttt_loopback";
static const int SHM_PROJ = 0x14;
static const int SEM_PROJ = 0x24;

static int shmid;
static int semid;

static GameInfo *game_info;
static GameSlot *slot;
static bool is_host;


int init_loopback() {
    is_host = false;

    creat(SHM_PATH, 0666);

    key_t sem_key = ftok(SHM_PATH, SEM_PROJ);
    if ((semid = semget(sem_key, 1, IPC_CREAT | 0666)) == -1) {
        perror("Unable to semget");
        return -1;
    }
    lock_sem(semid);

    key_t shm_key = ftok(SHM_PATH, SHM_PROJ);
    if ((shmid = shmget(shm_key, sizeof(GameInfo), IPC_CREAT | 0666)) == -1) {
        perror("Unable to shmget");
        return -1;
    }
    if ((game_info = shmat(shmid, NULL, 0)) == (void*) -1) {
        perror("Unable to shmat");
        return -1;
    }
    game_info->attached++;
    release_sem(semid);

    return 0;
}

void close_loopback() {
    lock_sem(semid);
    if (is_host && slot != NULL) {
        slot->state = SS_EMPTY;
    }
    int attached = --game_info->attached;
    release_sem(semid);
    shmdt(game_info);
    if (attached == 0) {
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
        remove(SHM_PATH);
    }
}

int host_game() {
    slot = NULL;
    lock_sem(semid);
    for (size_t i = 0; i < MAX_SLOTS; i++) {
        if (game_info->slots[i].state == SS_EMPTY) {
            slot = &game_info->slots[i];
            break;
        }
    }
    if (slot == NULL) {
        release_sem(semid);
        return -1;  // no available slots to host game
    }
    slot->state = SS_WAIT;
    slot->turn_done = false;
    slot->move_maker = CROSS;
    is_host = true;
    release_sem(semid);
    return 0;
}

int join_game() {
    slot = NULL;
    lock_sem(semid);
    for (size_t i = 0; i < MAX_SLOTS; i++) {
        if (game_info->slots[i].state == SS_WAIT) {
            slot = &game_info->slots[i];
            break;
        }
    }
    if (slot == NULL) {
        release_sem(semid);
        return -1;  // no slots to join
    }
    slot->state = SS_GAME;
    is_host = false;
    release_sem(semid);
    return 0;
}

int loopback_make_turn(GameLoop *gloop, Cell *turn) {
    int res = keyboard_turn(gloop, turn);
    lock_sem(semid);
    if (res == 0) {
        memcpy(&slot->turn, turn, sizeof(Cell));
        slot->turn_done = true;
    }
    release_sem(semid)
    return res;
}

int loopback_wait_turn(GameLoop *gloop, Cell *turn) {
    int res;
    lock_sem(semid);
    if (slot->turn_done && slot->move_maker != gloop->host_player) {
        memcpy(turn, &slot->turn, sizeof(Cell));
        slot->turn_done = false;
        slot->move_maker = gloop->host_player;
        res = 0;
    }
    else {
        res = 1;
    }
    release_sem(semid);
    return res;
}
