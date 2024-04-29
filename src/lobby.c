#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#include "lobby.h"
#include "utils.h"

static const char DIR_PATH[] = "/tmp/ttt";
static const char FIFO_PATH[] = "/tmp/ttt/fifo";


void init_loopback() {
    struct stat st;
    if (stat(DIR_PATH, &st) == -1) {
        mkdir(DIR_PATH, 0700);
    }
    if (mkfifo(FIFO_PATH, 0660) != 0) {
        perror("Unable to create fifo");
    }
}

void close_loopback() {
}

int host_game() {
    return 0;
}

int join_game() {
    return 0;
}

int loopback_turn(GameLoop *gloop, Cell *turn) {
    int res = keyboard_turn(gloop, turn);
    if (res == 0) {
        int fd = open(FIFO_PATH, O_WRONLY);
        if (write(fd, turn, sizeof(Cell)) == -1) {
            perror("Unable to write Cell in fifo");
        }
        close(fd);
    }
    return res;
}

int loopback_other_turn(GameLoop *gloop, Cell *turn) {
    int fd = open(FIFO_PATH, O_RDONLY);
    int retval = read(fd, turn, sizeof(Cell));
    close(fd);
    if (retval > 0) {
        return 0;
    }
    if (retval == -1) {
        perror("Unable to read Cell from fifo");
    }
    return 1;
}
