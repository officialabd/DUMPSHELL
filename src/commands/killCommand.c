#include "../../headers/commands/killCommand.h"

void killHandler(char **arg) {
    pid_t pid;
    int sig;
    if (!(arg[1])) { /** No arguments passed with the kill command.*/
        printf("\033[33mFew arguments\033[0m\n");
        printf("Try: \nkill pid\nor kill -l or kill -L\nor kill -SIGNAL pid0 pid1 pid2 ...\nor kill -NUMBER pid0 pid1 pid2 ...\n");
    } else if ((!strcmp(arg[1], "-l")) || (!strcmp(arg[1], "-L"))) { /** Print all signals that the shell can send.*/
        printSIGNALS();
    } else if (arg[1] && (arg[2] == NULL)) { /** Send SIGTERM to the pid provided by the user*/
        pid = atoi(arg[1]);
        if (pid == 0) {
            printf("Invalid argument\n  Try kill NUMBER\n");
            return;
        }
        sig = SIGTERM;
        ikill(pid, sig);

    } else { /** Send a signal that user specify to the 1 pid or more, provided by the user.*/
        if ((arg[1])[0] == '-') {
            sig = atoi((arg[1] + 1));
            if (sig == 0) {
                sig = findSig((arg[1] + 1));
                if (sig == NO_SIG_FOUND) {
                    printf("Invalid argument\n  Try -SIGNAL\n");
                }
            }
            if (sig != NO_SIG_FOUND) {
                int index = 2;
                while (arg[index] != NULL) {
                    pid = strtol(arg[index++], NULL, 10);
                    ikill(pid, sig);
                }
            }
        } else {
            printf("Invalid argument\n  Try -SIGNAL\n");
        }
    }
}

void ikill(pid_t pid, int sig) {
    if (kill(pid, sig) == -1) {
        char *temp = calloc(sizeof(char), 64);
        sprintf(temp, "ikill (pid: %d , sig: %d)", pid, sig);
        perror(temp);
        free(temp);
    }
}

int findSig(char *sig) {
    char signals[SIGNALS][10] = {
        {"SIGHUP\0"}, {"SIGINT\0"}, {"SIGQUIT\0"}, {"SIGILL\0"}, {"SIGTRAP\0"}, {"SIGABRT\0"}, {"SIGBUS\0"}, {"SIGFPE\0"}, {"SIGKILL\0"}, {"SIGUSR1\0"}, {"SIGSEGV\0"}, {"SIGUSR2\0"}, {"SIGPIPE\0"}, {"SIGALRM\0"}, {"SIGTERM\0"}, {"SIGSTKFLT\0"}, {"SIGCHLD\0"}, {"SIGCONT\0"}, {"SIGSTOP\0"}, {"SIGTSTP\0"}, {"SIGTTIN\0"}, {"SIGTTOU\0"}, {"SIGURG\0"}, {"SIGXCPU\0"}, {"SIGXFSZ\0"}, {"SIGVTALRM\0"}, {"SIGPROF\0"}, {"SIGWINCH\0"}, {"SIGPOLL\0"}, {"SIGPWR\0"}, {"SIGSYS\0"}};
    for (int i = 0; i < SIGNALS; i++) {
        if (!strcmp(signals[i], sig)) {
            return (i + 1);
        }
    }
    return NO_SIG_FOUND;
}

void printSIGNALS() {
    char signals[SIGNALS][10] = {{"SIGHUP\0"}, {"SIGINT\0"}, {"SIGQUIT\0"}, {"SIGILL\0"}, {"SIGTRAP\0"}, {"SIGABRT\0"}, {"SIGBUS\0"}, {"SIGFPE\0"}, {"SIGKILL\0"}, {"SIGUSR1\0"}, {"SIGSEGV\0"}, {"SIGUSR2\0"}, {"SIGPIPE\0"}, {"SIGALRM\0"}, {"SIGTERM\0"}, {"SIGSTKFLT\0"}, {"SIGCHLD\0"}, {"SIGCONT\0"}, {"SIGSTOP\0"}, {"SIGTSTP\0"}, {"SIGTTIN\0"}, {"SIGTTOU\0"}, {"SIGURG\0"}, {"SIGXCPU\0"}, {"SIGXFSZ\0"}, {"SIGVTALRM\0"}, {"SIGPROF\0"}, {"SIGWINCH\0"}, {"SIGPOLL\0"}, {"SIGPWR\0"}, {"SIGSYS\0"}};
    for (int r = 1; r <= SIGNALS; r++) {
        printf("%2d) %1s\t", r, signals[r - 1]);
        if ((r % 5) == 0)
            printf("\n");
    }
    printf("\n");
}