#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define BACKSPACE 127
#define MAX_LINE_SIZE 4096
#define CHAR_SIZE 1
#define ENTER 10

/**
 * It Handle the user input on Console
 * @param line It is a char pointer which contain the user input
 * @param size It is used to allocate char pointer in the memory of size
 * @return This function returns a char pointer to the user input   
 */
char *handleInput(char *line, int size);