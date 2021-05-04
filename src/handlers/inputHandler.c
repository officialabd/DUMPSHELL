#include "../../headers/handlers/inputHandler.h"

char *handleInput(char *cmdLine, int size) {
    /** Changing the attribute of the terminal*/
    struct termios new, old;
    tcgetattr(0, &old);
    new = old;
    new.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);

    int c;
    cmdLine = calloc(CHAR_SIZE, MAX_LINE_SIZE);

    int i = 0;
    while (1) {
        c = getchar();
        if (c >= 32 && c <= 126) {
            putchar(c);
            cmdLine[i++] = (char)c;
        } else if (c == BACKSPACE) {
            if (backspace(cmdLine, i)) {
                i--;
            }
        } else if (c == '\33') {
            char ch = getchar();
            if (ch == '[') {
                c = getchar();
                // tcsetattr(STDIN_FILENO, TCSANOW, &old);
                // printf("\n");
                continue;
                switch (c) {
                    case KEY_UP:
                        clear(cmdLine, i);
                        openHistory("r+");
                        cmdLine = getUp();
                        closeHistory();
                        printf("UP AF: cmdline: %s\n", cmdLine);
                        break;
                    case KEY_DOWN:
                        cmdLine = getDown();
                        printf("%s", cmdLine);
                        break;
                    case KEY_LEFT:
                        return LEFT;
                        break;
                    case KEY_RIGHT:
                        return RIGHT;
                        break;
                }
            }
        } else if (c == ENTER) {
            cmdLine[i] = '\0';
            tcsetattr(STDIN_FILENO, TCSANOW, &old);
            printf("\n");
            return cmdLine;
        }
    }
}

int backspace(char *cmdLine, int index) {
    // char temp[128];
    // sprintf(temp, "../test.txt");
    // FILE *f = fopen(temp, "a");
    // if (!f) {
    //     perror("Open history");
    //     return 0;
    // }
    // sprintf(temp, "%d\n", index);
    // fputs(temp, f);
    if (index > 0) {
        fflush(stdout);
        putchar('\b');
        putchar(' ');
        putchar('\b');
        // printf("\x1B[A");
        fflush(stdout);
        cmdLine[index] = 0;
        cmdLine[--index] = '\0';
        return 1;
    } else
        return 0;
}

void clear(char *cmdLine, int index) {
    while (index--) backspace(cmdLine, index);
}