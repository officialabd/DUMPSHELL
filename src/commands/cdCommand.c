#include "../../headers/commands/cdCommand.h"

void icd(char **args) {
    char des[MAX_PATH_SIZE];
    char newPath[MAX_PATH_SIZE];

    strcpy(newPath, getcwd_(MAX_PATH_SIZE));

    if (args[2] != NULL) { /* More than one arg passed */
        fprintf(stderr, "There are too many args\n");
        return;
    } else if (!args[1] || (args[1][0] == '~')) { /* To change the directory to the /home/user */
        char buf[MAX_PATH_SIZE];
        char desPth[] = "/home/";
        strcpy(buf, getlogin());
        strcat(desPth, buf);
        if (chdir(desPth) == -1) {
            perror("chdir");
        }
        return;
    } else if (!strcmp(args[1], "..")) {
        if (chdir("..") == -1) {
            perror("chdir");
        }
    } else if (args[1][0] != '/') { /** If user inputs sub or parent directory of the current directory */
        strcat(newPath, "/");
        strcat(newPath, args[1]);
        if (chdir(newPath) == -1) {
            perror("chdir");
        }
    } else { /** If user inputs a full path directory */
        if (chdir(args[1]) == -1) {
            perror("chdir");
        }
    }
}