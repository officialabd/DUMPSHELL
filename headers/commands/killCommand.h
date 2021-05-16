#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define NO_SIG_FOUND -2
#define SIGNALS 31
#define MAX_PATH_SIZE 1024

/**
 * It handle input ot kill 
 * @param arg the arguments of user input
 * @return no return value (void)
 */
void killHandler(char **arg);

/**
 * It sends a signal to a process 
 * @param pid the process id
 * @param sig The type of the signal that should be sent
 * @return no return value (void)
 */
void ikill(pid_t pid, int sig);

/**
 * It finds the number of the signal
 * @param sig The string of signal
 * @return return the number of the signal 
 */
int findSig(char *sig);

/**
 * It prints all signals that the kill command can send
 * @return no return value (void)
 */
void printSIGNALS();