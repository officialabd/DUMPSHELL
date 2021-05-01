#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/procfs.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../headers/commands/cdCommand.h"
#include "../headers/commands/psCommand.h"
#include "../headers/commands/pwdCommand.h"
#include "../headers/handlers/inputHandler.h"

#define DEBUG 1
#define MAXLINELEN 4096
#define MAXARGS 128
#define END_OF_LINE 0
#define SEQ_OP ';'
#define SEQUENCE 1
#define MAX_PATH_SIZE 1024
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_LEFT 67
#define KEY_RIGHT 68

struct cmd {
    struct cmd *next;
    int terminator;
    char *exe_path;
    int nargs;
    char *arg[MAXARGS];
};
/**
 * Allocate size bytes of memory.
*/
void *ck_malloc(size_t size) {
    void *ret = malloc(size);
    if (!ret) {
        perror("dumbshell:ck_malloc");
        exit(1);
    }
    return ret;
}

char *skip_to_non_ws(char *p) {
    int ch;
    while (ch = *p) {
        if (ch != ' ' && ch != '\t' && ch != '\n') return p;
        ++p;
    }
    return 0;
}

char *skip_to_ws_or_sep(char *p) {
    int ch;
    while (ch = *p) {
        if (ch == ' ' || ch == '\t' || ch == '\n') return p;
        if (ch == SEQ_OP) return p;
        ++p;
    }
    return 0;
}

struct cmd *parse_commands(char *line) {
    char *ptr;
    int ix;
    struct cmd *cur;

    ptr = skip_to_non_ws(line);
    if (!ptr) return 0;
    cur = ck_malloc(sizeof *cur);
    cur->next = 0;
    cur->exe_path = ptr;
    cur->arg[0] = ptr;
    cur->terminator = END_OF_LINE;
    ix = 1;
    for (;;) {
        ptr = skip_to_ws_or_sep(ptr);

        if (!ptr) {
            break;
        }
        if (*ptr == SEQ_OP) {  // ------------- check later

            *ptr = 0;
            cur->next = parse_commands(ptr + 1);
            if (cur->next) {
                cur->terminator = SEQUENCE;
            }
            break;
        }
        *ptr = 0;
        ptr = skip_to_non_ws(ptr + 1);
        if (!ptr) {
            break;
        }
        if (*ptr == SEQ_OP) {
            /* found a sequence operator */
            cur->next = parse_commands(ptr + 1);
            if (cur->next) {
                cur->terminator = SEQUENCE;
            }
            break;
        }
        cur->arg[ix] = ptr;
        ++ix;
    }
    cur->arg[ix] = 0;
    cur->nargs = ix;
    return cur;
}

int extra_commands(struct cmd *c) {
    char cmd[MAX_PATH_SIZE];

    strcpy(cmd, c->exe_path);

    if (!strcmp(cmd, "pwd")) {
        pwd(MAX_PATH_SIZE);
        return 1;
    } else if (!strcmp(cmd, "ps")) {
        ps(c->arg);
        return 1;
    } else if (!strcmp(cmd, "cd")) {
        return 1;
    } else {
        return 0;
    }
}

void execute(struct cmd *clist) {
    int pid, npid, stat;

    pid = fork();
    if (pid == -1) {
        perror("dumbshell:fork");
        exit(1);
    }

    if (!pid) {
        if (!extra_commands(clist)) {
            execvp(clist->exe_path, clist->arg);
            fprintf(stderr, "No such command: %s\n", clist->exe_path);
        }
        exit(1);
    } else {
        if (!strcmp(clist->exe_path, "cd")) {
            cd(clist->arg);
        }
    }
    do {
        npid = wait(&stat);
        printf("Process %d exited with status %d\n", npid, stat);
    } while (npid != pid);
    switch (clist->terminator) {
        case SEQUENCE:
            execute(clist->next);
    }
}
void free_commands(struct cmd *clist) {
    struct cmd *nxt;

    do {
        nxt = clist->next;
        free(clist);
        clist = nxt;
    } while (clist);
}

char *get_command(char *buf, int size, FILE *in) {
    if (in == stdin) {
        char *firstLine = calloc(CHAR_SIZE, MAX_PATH_SIZE);
        sprintf(firstLine, "~%s@ ", (getcwd_(MAX_PATH_SIZE) + 9));
        fputs(firstLine, stdout);
        free(firstLine);
    }
    buf = handleInput(buf, size);
    // printf("buf: (%s) ", buf);
    return buf;
}

void main(void) {
    char linebuf[MAXLINELEN];
    struct cmd *commands;
    char *ch = get_command(linebuf, MAXLINELEN, stdin);

    while (&ch != NULL) {
        commands = parse_commands(ch);
        if (commands) {
            execute(commands);
            free_commands(commands);
        }
        ch = get_command(linebuf, MAXLINELEN, stdin);
    }
}