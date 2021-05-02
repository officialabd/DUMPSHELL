#include "../../headers/commands/cdCommand.h"

void cd(char **args) {
    if (args[2] != NULL) { /* More than one arg passed */
        fprintf(stderr, "There are too many args\n");
        return;
    }

    if (!args[1] || (args[1][0] == '~')) { /* To change the directory to the /home/user */
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

    strcpy(des, args[1]);
    strcpy(newPath, getcwd_(MAX_PATH_SIZE));

    if (des[0] != '/') { /** If user inputs sub or parent directory of the current directory */
        strcat(newPath, "/");
        strcat(newPath, des);
        if (chdir(newPath) == -1) {
            perror("chdir");
        }
    } else { /** If user inputs a full path directory */
        if (chdir(des) == -1) {
            perror("chdir");
        }
    }
}