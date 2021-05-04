#include "../../headers/commands/pwdCommand.h"

void ipwd(size_t size) {
    char *dir = getcwd_(MAX_PATH_SIZE);
    if (dir != NULL)
        printf("%s \n", dir);
}

char *getcwd_(size_t size) {
    char *pdir = NULL;
    pdir = getcwd(pdir, size);
    if (pdir == NULL) {
        perror("getcwd");
    }
    strcat(pdir, "\0");
    return pdir;
}
