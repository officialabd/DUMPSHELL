#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "commandsHandler.h"

/**
 * Execute 1-3 commands with piping or redirection or both.
 * @param cmd_1 First command to execute. 
 * @param cmd_2 Second command to execute. 
 * @param cmd_3 Third command to execute. 
 * @param count Number of commands.
 * @param redirect 0 or 1, tell if there is redirection.
 * @param endPath if redirect is 1 then endPath is the path of file to redirecte the output to it. 
 * @return no return value (void)
 */
void ipipe(struct cmd *cmd_1, struct cmd *cmd_2, struct cmd *cmd_3, int count, int redirect, char *endPath);
