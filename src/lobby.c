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

static int wr_fifo = 0;
static int rd_fifo = 0;
static bool is_host = false;

static char host_fifo_name[] = "/tmp/ttt/host";
static char client_fifo_name[] = "/tmp/ttt/client";


void init_loopback() {
    struct stat st;
    if (stat(DIR_PATH, &st) == -1) {
        mkdir(DIR_PATH, 0700);
    }
}

void close_loopback() {
    close(wr_fifo);
    close(rd_fifo);

    if (is_host) {
        remove(host_fifo_name);
        remove(client_fifo_name);
    }
}

int host_game() {
    if (mkfifo(host_fifo_name, 0660) != 0) {
        perror("Unable to create write fifo");
        return -1;
    }
    if (mkfifo(client_fifo_name, 0660) != 0) {
        perror("Unable to create read fifo");
        return -1;
    }
    wr_fifo = open(host_fifo_name, O_WRONLY);
    rd_fifo = open(client_fifo_name, O_RDONLY);

    int flags = fcntl(rd_fifo, F_GETFL, 0);
    fcntl(rd_fifo, F_SETFL, flags | O_NONBLOCK);

    is_host = true;
    return 0;
}

int join_game() {
    wr_fifo = open(client_fifo_name, O_WRONLY);
    rd_fifo = open(host_fifo_name, O_RDONLY);

    int flags = fcntl(rd_fifo, F_GETFL, 0);
    fcntl(rd_fifo, F_SETFL, flags | O_NONBLOCK);

    return 0;
}

int loopback_turn(GameLoop *gloop, Cell *turn) {
    int res = keyboard_turn(gloop, turn);
    if (res == 0) {
        if (write(wr_fifo, turn, sizeof(Cell)) == -1) {
            perror("Unable to write to wr_fifo");
        }
    }
    return res;
}

int loopback_other_turn(GameLoop *gloop, Cell *turn) {
    int retval = read(rd_fifo, turn, sizeof(Cell));
    if (retval > 0) {
        return 0;
    }
    if (retval == -1) {
        perror("Unable to read from rd_fifo");
    }
    return 1;
}
