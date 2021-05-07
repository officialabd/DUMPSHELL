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

#include "../headers/handlers/commandsHandler.h"
#include "../headers/handlers/inputHandler.h"

#define DEBUG 1
#define MAXLINELEN 4096
#define END_OF_LINE 0
#define SEQ_OP ';'
#define SEQUENCE 1
#define MAX_PATH_SIZE 1024
#define UP "UP"
#define DOWN "DOWN"
#define LEFT "LEFT"
#define RIGHT "RIGHT"

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
        if (*ptr == SEQ_OP) {
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

        cur->arg[ix] = ptr;
        ++ix;
    }
    cur->arg[ix] = 0;
    cur->nargs = ix;
    return cur;
}

void execute(struct cmd *clist) {
    int pid, npid, stat;

    pid = fork();
    if (pid == -1) {
        perror("dumbshell:fork");
        exit(1);
    }

    if (!pid) {
        if (!handle(clist)) {
            execvp(clist->exe_path, clist->arg);
            fprintf(stderr, "No such command: %s\n", clist->exe_path);
        }
        exit(1);
    } else {
        if (!strcmp(clist->exe_path, "cd")) {
            icd(clist->arg);
        }
    }
    do {
        npid = wait(&stat);
        if (strcmp(clist->exe_path, "clear"))
            printf(">\033[36mProcess\033[0m %d \033[36mexited with status\033[0m %d\n", npid, stat);
    } while (npid != pid);
    switch (clist->terminator) {
        case SEQUENCE:
            execute(clist->next);
    }
}

// cd home
// kill print
void free_commands(struct cmd *clist) {
    struct cmd *nxt;

    do {
        nxt = clist->next;
        free(clist);
        clist = nxt;
    } while (clist);
}

char *get_command(char *buf, int size, FILE *in) {
    int prev = 0;
    if (in == stdin) {
        char *username = getlogin();
        char *path = getcwd_(MAX_PATH_SIZE);
        if (strlen(path) >= (6 + strlen(username))) path += 9;
        char *firstLine = calloc(CHAR_SIZE, MAX_PATH_SIZE);
        sprintf(firstLine, "\033[31m%s@%s\033[0m:\033[32m~%s\033[0m@ ", username, username, path);
        prev = (2 * strlen(username)) + strlen(path) + 4;
        fputs(firstLine, stdout);
        free(firstLine);
    }
    buf = handleInput(buf, size, prev);
    return buf;
}

void main(void) {
    initHistory();
    char linebuf[MAXLINELEN];
    struct cmd *commands;

    char *ch = "clear";
    commands = parse_commands(ch);
    execute(commands);

    ch = get_command(linebuf, MAXLINELEN, stdin);
    while (&ch != NULL) {
        commands = parse_commands(ch);
        if (commands) {
            execute(commands);
            free_commands(commands);
        }
        ch = get_command(linebuf, MAXLINELEN, stdin);
    }
}