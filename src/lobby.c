#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#include "lobby.h"


static const char FIFO_PREFIX_PATH[] = "/var/run/ttt";


void check_lobby_path() {
    if (access(FIFO_PREFIX_PATH, R_OK | W_OK | X_OK) == -1) {
        mkdir(FIFO_PREFIX_PATH, S_IRWXU);
    }
}

int try_get_lobby(Lobby *lobby) {
    DIR *dir = opendir(FIFO_PREFIX_PATH);
    struct dirent ent;
    while ((ent = readdir(dir)) ) 
}

int create_new_lobby(Lobby *lobby) {

}
