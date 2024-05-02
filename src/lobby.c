#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#include "lobby.h"
#include "utils.h"

static const char DIR_PATH[] = "/tmp/ttt";
static const char FIFO_HOST[] = "/tmp/ttt/host";
static const char FIFO_CLNT[] = "/tmp/ttt/clnt";

static int read_fd;
static int write_fd;

void init_loopback() {
    struct stat st;
    if (stat(DIR_PATH, &st) == -1) {
        mkdir(DIR_PATH, 0700);
    }
    if (access(FIFO_HOST, R_OK | W_OK) != 0 && mkfifo(FIFO_HOST, 0660) != 0) {
        perror("Unable to create host fifo");
    }
    if (access(FIFO_CLNT, R_OK | W_OK) != 0 && mkfifo(FIFO_CLNT, 0660) != 0) {
        perror("Unable to create client fifo");
    }
    //sigaction(SIGPIPE, &(struct sigaction){SIG_IGN}, NULL);
}

void close_loopback() {
    close(read_fd);
    close(write_fd);
}

int host_game() {
    read_fd = open(FIFO_CLNT, O_RDONLY | O_NONBLOCK);
    write_fd = open(FIFO_HOST, O_WRONLY);
    return 0;
}

int join_game() {
    write_fd = open(FIFO_CLNT, O_WRONLY);
    read_fd = open(FIFO_HOST, O_RDONLY | O_NONBLOCK);
    return 0;
}

int loopback_turn(GameLoop *gloop, Cell *turn) {
    int res = keyboard_turn(gloop, turn);
    if (res == 0) {
        if (write(write_fd, turn, sizeof(Cell)) == -1) {
            perror("Unable to write Cell in fifo");
        }
    }
    return res;
}

int loopback_other_turn(GameLoop *gloop, Cell *turn) {
    int retval = read(read_fd, turn, sizeof(Cell));
    if (retval > 0) {
        return 0;
    }
    return 1;
}
