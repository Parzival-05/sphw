#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#define DATA_FILE "data.txt"
#define TEMP_FILE "data.tmp"
#define PING_FLAG "ping.flag"
#define PONG_FLAG "pong.flag"
#define SLEEP_US 100000


static int file_exists(const char *path) {
    struct stat sb;
    return stat(path, &sb) == 0;
}

static int create_flag(const char *flag) {
    int fd = open(flag, O_WRONLY | O_CREAT | O_EXCL, 0600);
    if (fd >= 0) {
        close(fd);
        return 0;
    }
    return -1;
}

int main(int argc, char **argv) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s <positive-iterations>\n", argv[0]);
        return 1;
    }
    int iterations = atoi(argv[1]);
    if (iterations <= 0) {
        fprintf(stderr, "Iterations must be a positive integer\n");
        return 1;
    }
    setvbuf(stdout, NULL, _IOLBF, 0);

    for (int i = 1; i <= iterations; i++) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Message %d", i);
        FILE *f = fopen(TEMP_FILE, "w");
        if (!f) return 1;
        fprintf(f, "%s\n", msg);
        fflush(f);
        fsync(fileno(f));
        fclose(f);
        rename(TEMP_FILE, DATA_FILE);

        create_flag(PING_FLAG);
        printf("[PING] Записал данные: %s\n", msg);
        fflush(stdout);

        while (!file_exists(PONG_FLAG))
            usleep(SLEEP_US);

        unlink(PONG_FLAG);
    }

    return 0;
}