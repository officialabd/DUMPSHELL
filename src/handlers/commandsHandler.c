#include "../../headers/handlers/commandsHandler.h"

int handle(struct cmd *c) {
    char cmd[MAX_PATH_SIZE];

    strcpy(cmd, c->exe_path);

    if (!strcmp(cmd, "pwd")) {
        ipwd(MAX_PATH_SIZE);
        return 1;
    } else if (!strcmp(cmd, "ps")) {
        ips(c->arg);
        return 1;
    } else if (!strcmp(cmd, "cd")) {
        return 1;
    } else if (!strcmp(cmd, "kill")) {
        killHandler(c->arg);
        return 1;
    } else {
        return 0;
    }
}
