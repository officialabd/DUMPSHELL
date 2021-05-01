#include "../../headers/handlers/inputHandler.h"

char *handleInput(char *line, int size) {
    int c;
    struct termios new, old;
    tcgetattr(0, &old);
    new = old;
    new.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
    char *cmdLine = calloc(CHAR_SIZE, MAX_LINE_SIZE);

    int i = 0;
    while (1) {
        c = getchar();
        // printf("c %d ", c);
        if (c >= 32 && c <= 126) {  // ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 32)
            putchar(c);
            cmdLine[i++] = (char)c;
        } else if (c == BACKSPACE) {
            if (i > 0) {
                fflush(stdout);
                putchar('\b');
                putchar(' ');
                putchar('\b');
                fflush(stdout);
                cmdLine[i] = 0;
                cmdLine[--i] = '\0';
            }
        } else if (c == '\33') {
            char ch = getchar();
            if (ch == '[') {
                c = getchar();
                cmdLine[i++] = c;
                cmdLine[i] = '\0';
                tcsetattr(STDIN_FILENO, TCSANOW, &old);
                printf("\n");
                return cmdLine;
            }
        } else if (c == ENTER) {
            cmdLine[i] = '\0';
            tcsetattr(STDIN_FILENO, TCSANOW, &old);
            printf("\n");
            return cmdLine;
        }
    }
}