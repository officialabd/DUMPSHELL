#include "../../headers/commands/psCommand.h"

void ips(char **args) {
    printf("%6s%12s\t%1s\t%12s\t%s\n", "PID", "TTY", "STATE", "TIME", "CMD");

    if (!(args[1])) {
        printProc(getppid());
        printProc(getpid());
    } else if (!(strcmp(args[1], "-A")) || !(strcmp(args[1], "-a"))) {
        struct dirent *sd;
        DIR *dir = opendir("/proc");
        if (dir == NULL) {
            perror("DIR: Unable to open directory");
            exit(EXIT_FAILURE);
        }

        while ((sd = readdir(dir)) != NULL) {
            if ((sd->d_type == DT_DIR)) {
                int pid = strtol(sd->d_name, NULL, 10);
                if (pid > 0) {
                    printProc(pid);
                }
            }
        }
    } else {
        int i = 1;
        while (args[i] != NULL) {
            int pid = strtol(args[i++], NULL, 10);
            printProc(pid);
        }
    }
}

void printProc(int pid) {
    char path[1000];
    sprintf(path, "/proc/%d/stat", pid);
    FILE *f = fopen(path, "r");
    if (!f) {
        perror("Open directory");
        return;
    }
    char **data = getStat(data, pid);

    char *tty = data[0];
    char *state = data[1];
    char *time = data[2];
    char *cmd = data[3];

    printf("%6d%12s\t%1s\t%12s\t%s\n", pid, tty, state, time, cmd);

    fclose(f);
}

char **getStat(char **data, int pid) {
    char statPath[MAX_PATH_SIZE], fdPath[MAX_PATH_SIZE];

    sprintf(statPath, "/proc/%d/stat", pid);
    FILE *statFile = fopen(statPath, "r");

    sprintf(fdPath, "/proc/%d/fd/%d", pid, 0);
    int fdFile = open(fdPath, O_RDONLY);

    if (!statFile) {
        perror("STAT File");
        return NULL;
    }
    data = malloc(sizeof(char *) * 4);
    if (!data)
        return NULL;
    for (int j = 0; j < 4; j++) {
        data[j] = malloc(100);
        if (!data[j]) {
            free(data);
            return NULL;
        }
    }

    char cmd[64];
    char state;
    unsigned long long tty;  //long long
    unsigned long utime, stime;

    fscanf(statFile, "%*d %s %c %*d %*d %*d %llu %*d %*d %*d %*d %*d %*d %lu %lu", cmd, &state, &tty, &utime, &stime);

    char *tty2 = ttyname(fdFile);
    if (!tty2) {
        tty2 = "     ?";
    }
    sprintf(data[0], "%s", tty2 + 5);
    sprintf(data[1], "%c", state);
    sprintf(data[2], "%s", timeParser(data[2], (utime + stime)));
    sprintf(data[3], "%s", cmd);

    fclose(statFile);

    return data;
}

char *timeParser(char *ptime, unsigned long time) {
    long ticksPerSec = sysconf(_SC_CLK_TCK);

    int msecs = ((time / (double)ticksPerSec) - (time / ticksPerSec)) * ticksPerSec;
    int totSecs = time / ticksPerSec;
    int totMins = totSecs / 60;
    int secs = (time / ticksPerSec) - ((int)(time / (ticksPerSec * 60)) * 60);
    int mins = totSecs / 60 - ((int)(totSecs / (60 * 60)) * 60);
    int hours = totMins / 60;

    ptime = malloc(1028);
    if (!ptime) {
        free(ptime);
        return NULL;
    }
    sprintf(ptime, "%.2d:%.2d:%.2d:%.2d", hours, mins, secs, msecs);

    return ptime;
}