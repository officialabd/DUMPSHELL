#include "../../headers/commands/cdCommand.h"

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