#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH_SIZE 1024
#define HISTORY_PATH "/home/abd/Documents/VSCode-Workspace/dumpshell/history/hist.hi"
#define HISTORY_DATA_PATH "/home/abd/Documents/VSCode-Workspace/dumpshell/history/historyData.da"

FILE *hist; /** File pointer to the history file (/history/hist.hi)*/

/**
 * This function get the previos used command from the current command if available. 
 * See also open(), close()
 * @param[in] NoParameters
 * @param[out] cmdLine 
 * @return Char pointer to command line
 */
char *get(int index);

/**
 * This function get the next from the current command if available. 
 * See also open(), close()
 * @param[in] NoParameters
 * @param[out] cmdLine 
 * @return Char pointer to command line
 */
char *getDown();

/**
 * Prints cmdLine to the history file. 
 * See also open(), close()
 * @param[in] cmdLine This command line will be printed to the history file
 * @return No return value (void)
 */
void put(char *cmdLine);

/**
 * Before calling getUp(), getDown() or put(), You should call this function to open the history file
 * @param[in] type Decide how the file should be (r, r+, w, w+, a, a+) 
 * @return No return value (void)
 */
void openHistory(char *type);

/**
 * After calling getUp(), getDown() or put(), You should call this function to close the history file
 * @return No return value (void)
 */
void closeHistory();

/**
 * It is used to update any data to history file
 * @return No return value (void)
 */
void update();

/**
 * Check all the required to use the history (length, ..)
 * YOU SHOULD USE THIS FUNCTION BEFORE ANY USAGE OF HISTORY
 * @return No return value (void)
 */
void initHistory();

/**
 * Number of commands
 * YOU SHOULD USE THIS FUNCTION BEFORE ANY USAGE OF HISTORY
 * @return int The total number of commands in the history.
 */
int getLength();