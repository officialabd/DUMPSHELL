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
 * It Handle the user input on Console.
 * @param cmdLine It is a char pointer which contain the user input.
 * @param size It is used to allocate char pointer in the memory of size.
 * @return This function returns a char pointer to the user input.
 */
char *handleInput(char *cmdLine, int size, int prev);

/**
 * It remove the last character.
 * @param cmdLine The command line to remove the last character from.
 * @param index the current index of the cursor.
 * @return return 1 on success, 0 on no space removed.
 */
int backspace(char *cmdLine, int index);

/**
 * Clear all user input.
 * @param cmdLine The command line to remove.
 * @param index the current index of the cursor.
 * @return no return value (void).
 */
void clear(char *cmdLine, int index);

/**
 * Move the cursor upward.
 * @param x number of lines to move the cursor.
 * @return no return value (void).
 */
void moveUp(int x);

/**
 * Move the cursor down ward.
 * @param x number of lines to move the cursor.
 * @return no return value (void).
 */
void moveDown(int x);

/**
 * Move the cursor to right.
 * @param x number of characters to move the cursor to.
 * @param cursoer_index Current cursor index.
 * @param chars_no The total number of characters of user input. 
 * @return int return the new index of cursor.
 */
int moveRight(int cursor_index, int chars_no, int x);

/**
 * Move the cursor to left.
 * @param x number of characters to move the cursor to.
 * @param cursoer_index Current cursor index.
 * @param chars_no The total number of characters of user input. 
 * @return int return the new index of cursor.
 */
int moveLeft(int cursor_index, int chars_no, int x);