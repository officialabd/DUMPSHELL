#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH_SIZE 1024

/**
 * Implementation of the PS command
 * @param[in] args User input arguments (flags for ps) 
 * @return No return value (void)
 */
void ips(char **args);

/**
 * It prints all data(PID, TTY, STATE, TIME, CMD) of the process
 * @param[in] pid Process id to print its data 
 * @return No return value (void) 
 */
void printProc(int pid);

/**
 * It gets all required data of a process from /proc folder.
 * @param[in] data String pointer to the data of the process. 
 * @param[in] pid Process id to get all data of it.
 * @return String pointer to the data of the process.
 */
char **getStat(char **data, int pid);

/**
 * This function takes the passed time of a process and formats it as HOURS:MINUTES:SECONDS:MILLISECONDS.
 * @param[in] ptime Char pointer to the formatted time.
 * @param[in] time The time that should be formatted
 * @return Char pointer to the formatted time 
 */
char *timeParser(char *ptime, unsigned long time);
