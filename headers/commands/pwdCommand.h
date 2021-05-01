#include <stdio.h>
#include <unistd.h>

#define MAX_PATH_SIZE 1024

char *getcwd_(size_t size);
/**
 *  Prints the current path of the working directory 
 */
void pwd(size_t size);