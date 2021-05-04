#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pwdCommand.h"

/**
 * This function change the current directory path to the new path
 * @param[in] args User input arguments provided with cd command for the new path
 * @return No return value (void)
 */
void icd(char **args);