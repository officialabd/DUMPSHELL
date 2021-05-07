#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "../../headers/handlers/historyHandler.h"

#define BACKSPACE 127
#define MAX_LINE_SIZE 4096
#define CHAR_SIZE 1
#define ENTER 10
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68

#define UP "UP"
#define DOWN "DOWN"
#define LEFT "LEFT"
#define RIGHT "RIGHT"

/**
 * It Handle the user input on Console
 * @param cmdLine It is a char pointer which contain the user input
 * @param size It is used to allocate char pointer in the memory of size
 * @return This function returns a char pointer to the user input   
 */
char *handleInput(char *cmdLine, int size, int prev);

/**
 * It remove the last character
 * @param cmdLine The command line to remove the last character from 
 * @param index the current index of the command line
 * @return return 1 on success, 0 on no space removed
 */
int backspace(char *cmdLine, int index);

void clear(char *cmdLine, int index);

void moveUp(int x);

void moveDown(int x);

int moveRight(int cursor_index, int chars_no, int x);

int moveLeft(int cursor_index, int chars_no, int x);