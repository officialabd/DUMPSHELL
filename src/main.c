#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define DEBUG 1
#define MAXLINELEN 4096
#define MAXARGS 128
#define END_OF_LINE 0
#define SEQ_OP ';'
#define SEQUENCE 1
#define MAX_PATH_SIZE 1024

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

char *getcwd_(size_t size) {
    char *pdir = NULL;
    pdir = getcwd(pdir, size);
    if (pdir == NULL) {
        perror("getcwd");
    }
    return pdir;
}
/**
 *  Prints the current path of the working directory 
 */
void pwd(size_t size) {
    char *dir = getcwd_(MAX_PATH_SIZE);
    if (dir != NULL)
        printf("Current directory: %s \n", dir);
}

void cd(char **path) {
    if (path[2] != NULL) { /* More than one arg passed */
        fprintf(stderr, "There are too many args\n");
        return;
    }

    if (!path[1] || (path[1][0] == '~')) { /* To change the directory to the /home/user */
        char buf[MAX_PATH_SIZE];
        char desPth[] = "/home/";
        strcpy(buf, getlogin());
        strcat(desPth, buf);
        if (chdir(desPth) == -1) {
            perror("chdir");
        }
        return;
    }

    char des[MAX_PATH_SIZE];
    char newPath[MAX_PATH_SIZE];

    strcpy(des, path[1]);
    strcpy(newPath, getcwd_(MAX_PATH_SIZE));

    if (des[0] != '/') {
        strcat(newPath, "/");
        strcat(newPath, des);
        if (chdir(newPath) == -1) {
            perror("chdir");
        }
    } else {
        if (chdir(des) == -1) {
            perror("chdir");
        }
    }
}

int extra_commands(struct cmd *c) {
    char cmd[MAX_PATH_SIZE];

    strcpy(cmd, c->exe_path);

    if (!strcmp(cmd, "pwd")) {
        pwd(MAX_PATH_SIZE);
        return 1;
    } else if (!strcmp(cmd, "cd")) {
        cd(c->arg);
        return 1;
    }

    return 0;
}

void execute(struct cmd *clist) {
    int npid, stat;

    if (!extra_commands(clist)) {
        execvp(clist->exe_path, clist->arg);
        fprintf(stderr, "No such command: %s\n", clist->exe_path);
        exit(1);
    }

    npid = wait(&stat);
    printf("Process %d exited with status %d\n", npid, stat);

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
        fputs("@ ", stdout); /* prompt */
    }
    return fgets(buf, size, in);
}

void main(void) {
    char linebuf[MAXLINELEN];
    struct cmd *commands;
    char *ch = get_command(linebuf, MAXLINELEN, stdin);
    while (&ch != NULL) {
        commands = parse_commands(linebuf);
        if (commands) {
            execute(commands);
            free_commands(commands);
        }
        ch = get_command(linebuf, MAXLINELEN, stdin);
    }
}