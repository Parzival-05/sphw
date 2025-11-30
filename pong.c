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
#define PING_FLAG "ping.flag"
#define PONG_FLAG "pong.flag"
#define SLEEP_US 100000

static int file_exists(const char *path) {
    struct stat sb;
    return stat(path, &sb) == 0;
}

static int create_flag(const char *flag) {
    int fd = open(flag, O_WRONLY | O_CREAT | O_EXCL, 0600);
    if (fd < 0) return -1;
    close(fd);
    return 0;
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
        while (!file_exists(PING_FLAG))
            usleep(SLEEP_US);

        FILE *f = fopen(DATA_FILE, "r");
        if (f) {
            char buf[256];
            if (fgets(buf, sizeof(buf), f)) {
                buf[strcspn(buf, "\n")] = '\0';
                printf("[PONG] Прочитал данные: %s\n", buf);
                fflush(stdout);
            }
            fclose(f);
        } else {
            fprintf(stderr, "[PONG] Warning: couldn't open %s: %s\n", DATA_FILE, strerror(errno));
        }

        unlink(PING_FLAG);
        create_flag(PONG_FLAG);
    }

    return 0;
}
