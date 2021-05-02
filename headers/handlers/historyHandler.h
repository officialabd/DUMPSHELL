#include <stdio.h>

int current = 0; /** Used to determin the current index of the cmdLine*/

/**
 * This function get the previos used command from the current command if available
 * @param[in] NoParameters
 * @param[out] cmdLine 
 * @return Char pointer to command line
 */
char *getUp();

/**
 * This function get the next from the current command if available
 * @param[in] NoParameters
 * @param[out] cmdLine 
 * @return Char pointer to command line
 */
char *getDown();

/**
 * Prints cmdLine to the history file
 * @param[in] cmdLine This command line will be printed to the history file
 * @return No return value (void)
 */
void put(char *cmdLine);