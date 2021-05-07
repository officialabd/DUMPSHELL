#include "../../headers/handlers/inputHandler.h"

char *handleInput(char *cmdLine, int size, int prev) {
    /** Changing the attribute of the terminal*/
    struct termios new, old;
    tcgetattr(0, &old);
    new = old;
    new.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);

    cmdLine = calloc(CHAR_SIZE, MAX_LINE_SIZE);

    char *temp = calloc(CHAR_SIZE, MAX_LINE_SIZE);
    char *reTemp = calloc(CHAR_SIZE, MAX_LINE_SIZE);

    int chars_no = 0, cursor_index = 0, histIndex = -1, c;
    while (1) {
        c = getchar();
        if (c >= 32 && c <= 126) {
            putchar(c);
            cmdLine[chars_no] = (char)c;
            temp[chars_no++] = (char)c;
            cursor_index++;
            histIndex = 0;
        } else if (c == BACKSPACE) {
            if (backspace(cmdLine, cursor_index)) {
                chars_no--;
                cursor_index--;
                histIndex = 0;
                cmdLine[cursor_index + 1] = 0;
                cmdLine[cursor_index] = '\0';
            }
        } else if (c == '\33') {
            char ch = getchar();
            if (ch == '[') {
                c = getchar();
                switch (c) {
                    case KEY_UP:
                        if (histIndex < getLength()) {
                            reTemp = getUp(++histIndex);
                            if (reTemp) {
                                cmdLine = reTemp;
                                clear(NULL, chars_no + 1);
                                printf("%s", cmdLine);
                                chars_no = strlen(cmdLine);
                                cursor_index = chars_no;
                            }
                        }
                        if (histIndex == getLength()) histIndex--;
                        break;
                    case KEY_DOWN:
                        if (--histIndex >= 0) {
                            reTemp = getUp(histIndex);
                            if (reTemp) {
                                cmdLine = reTemp;
                                clear(NULL, chars_no + 1);
                                chars_no = strlen(cmdLine);
                                cursor_index = chars_no;
                                printf("%s", cmdLine);
                            }
                        } else {
                            histIndex = -1;
                            cmdLine = temp;
                            clear(NULL, chars_no + 1);
                            chars_no = strlen(cmdLine);
                            cursor_index = chars_no;
                            printf("%s", cmdLine);
                        }
                        break;
                    case KEY_LEFT:
                        cursor_index = moveLeft(cursor_index, chars_no, 1);
                        histIndex = 0;
                        break;
                    case KEY_RIGHT:
                        cursor_index = moveRight(cursor_index, chars_no, 1);
                        histIndex = 0;
                        break;
                }
            }
        } else if (c == ENTER) {
            cmdLine[chars_no] = '\0';
            tcsetattr(STDIN_FILENO, TCSANOW, &old);
            put(cmdLine);
            printf("\n");
            histIndex = 0;
            return cmdLine;
        }
    }
}

int backspace(char *cmdLine, int index) {
    if (index > 0) {
        fflush(stdout);
        putchar('\b');
        putchar(' ');
        putchar('\b');
        fflush(stdout);
        if (cmdLine) {
            cmdLine[index] = 0;
            cmdLine[--index] = '\0';
        }
        return 1;
    } else
        return 0;
}

void clear(char *cmdLine, int index) {
    while (index--) backspace(cmdLine, index);
}

void moveUp(int x) {
    printf("\033[%dA", x);
}

void moveDown(int x) {
    printf("\033[%dB", x);
}

int moveRight(int cursor_index, int chars_no, int x) {
    if ((cursor_index < chars_no)) {
        printf("\033[%dC", x);
        cursor_index += x;
    }
    return cursor_index;
}

int moveLeft(int cursor_index, int chars_no, int x) {
    if (cursor_index > 0) {
        printf("\033[%dD", x);
        cursor_index -= x;
    }
    return cursor_index;
}
