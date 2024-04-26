#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>

#include "lobby.h"

static const char SHMEM_FILE[] = "/var/run/ttt_lmgr.shmem";
static const int SHMEM_ID = 0x14;
static const mode_t SHMEM_MODE = 0644;
static const size_t SHMEM_SIZE = sizeof(LobbyManager);
static int shmid = -1;

LobbyManager *lmgr_get() {
    if (access(SHMEM_FILE, F_OK) != 0) {
        creat(SHMEM_FILE, SHMEM_MODE);
    }
    key_t ipc_key = ftok(SHMEM_FILE, SHMEM_ID);
    shmid = shmget(ipc_key, SHMEM_SIZE, IPC_CREAT | SHMEM_MODE);
    if (shmid == -1) {
        perror("Unable to access shared memory region with LobbyManager");
        return NULL;
    }
    LobbyManager *lmgr = shmat(shmid, NULL, 0);
    if (lmgr == (void*) -1) {
        perror("Unable to access shared memory region with LobbyManager");
        return NULL;
    }
    return lmgr;
}

bool lmgr_need_remove(LobbyManager *lmgr) {
    bool flag = true;
    for (size_t i = 0; i < MAX_LOBBIES; i++) {
        flag = flag && (lmgr->lobbies[i].status == LS_CLOSE);
    }
    return flag;
}

void lmgr_free(LobbyManager *lmgr) {
    bool need_remove = false;
    if (lmgr_need_remove(lmgr)) {
        need_remove = true;
    }
    shmdt(lmgr);
}

Lobby *get_lobbies(LobbyManager *lmgr, int status) {

}
