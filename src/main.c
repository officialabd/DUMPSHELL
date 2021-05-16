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

#include "../headers/handlers/inputHandler.h"
#include "../headers/handlers/pipeHandler.h"

#define DEBUG 1
#define MAXLINELEN 4096
#define END_OF_LINE 0
#define SEQ_OP ';'
#define SEQUENCE 1
#define MAX_PATH_SIZE 1024

int parsePipe(char *cmdLine);

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
        if ((*ptr == SEQ_OP)) {
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
/**
 * This function parse the user input into commands
 * @param cmdLine String the user input
 * @return int (0 or 1) return 1 if there are pipe or redirection or both, 0 if there is no pipe and redirection
*/
int parsePipe(char *cmdLine) {
    char *cmdtemp = (char *)calloc(sizeof(char), MAX_LINE_SIZE);
    strcpy(cmdtemp, cmdLine);

    int isTherePipe = 0, isThereRedirection = 0;
    struct cmd *cmd_1, *cmd_2, *cmd_3;

    cmd_1 = ck_malloc(sizeof *cmd_1);
    cmd_1->next = 0;
    cmd_1->terminator = END_OF_LINE;
    cmd_1->exe_path = (char *)ck_malloc(MAX_LINE_SIZE);

    cmd_2 = ck_malloc(sizeof *cmd_2);
    cmd_2->next = 0;
    cmd_2->terminator = END_OF_LINE;
    cmd_2->exe_path = (char *)ck_malloc(MAX_LINE_SIZE);

    cmd_3 = ck_malloc(sizeof *cmd_3);
    cmd_3->next = 0;
    cmd_3->terminator = END_OF_LINE;
    cmd_3->exe_path = (char *)ck_malloc(MAX_LINE_SIZE);

    char *temp = (char *)calloc(sizeof(char), MAX_LINE_SIZE);
    char *endFD = NULL;

    int index = 0, count = 0;
    while (temp = strsep(&cmdtemp, " ")) {
        if (!strcmp(temp, "|")) {
            index = 0;
            isTherePipe++;
        } else if (!strcmp(temp, ">")) {
            isThereRedirection = 1;
            endFD = (char *)calloc(sizeof(char), MAX_LINE_SIZE);

            strcpy(endFD, cmdtemp);
            break;
        } else {
            if (isTherePipe == 0) {
                cmd_1->arg[index] = temp;
                if (index == 0) {
                    count++;
                    strcpy(cmd_1->exe_path, temp);
                }
            } else if (isTherePipe == 1) {
                cmd_2->arg[index] = temp;
                if (index == 0) {
                    count++;
                    strcpy(cmd_2->exe_path, temp);
                }
            } else {
                cmd_3->arg[index] = temp;
                if (index == 0) {
                    count++;
                    strcpy(cmd_3->exe_path, temp);
                }
            }
            index++;
        }
        temp = (char *)ck_malloc(MAX_LINE_SIZE);
    }
    if (isTherePipe == 0) {
        cmd_2 = NULL;
        cmd_3 = NULL;
    }
    if (isTherePipe == 1) {
        cmd_3 = NULL;
    }
    if ((isTherePipe || isThereRedirection)) {
        ipipe(cmd_1, cmd_2, cmd_3, count, isThereRedirection, endFD);
    }
    return (isTherePipe || isThereRedirection);
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
    int prev = 0;
    if (in == stdin) {
        char *username = getlogin();
        char *path = getcwd_(MAX_PATH_SIZE);
        if (strlen(path) >= (6 + strlen(username))) path += 6 + strlen(username);
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

    char *ch = calloc(sizeof(char), MAX_LINE_SIZE);

    ch = get_command(linebuf, MAXLINELEN, stdin);

    int i = 0;
    while (&ch != NULL) {
        if (!parsePipe(ch)) {
            commands = parse_commands(ch);
            if (commands) {
                execute(commands);
                free_commands(commands);
            }
        }
        ch = get_command(linebuf, MAXLINELEN, stdin);
    }
}
