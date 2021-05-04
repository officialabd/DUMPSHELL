#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH_SIZE 1024

/**
 * Prints the current path of the working directory
 * @param[in] size The size of the path  
 * @return No return value (void)
 */
void ipwd(size_t size);

/**
 * This function get the current directory path
 * @param[in] size The size of the path  
 * @return Returns char pointer to the current path 
 */
char *getcwd_(size_t size);