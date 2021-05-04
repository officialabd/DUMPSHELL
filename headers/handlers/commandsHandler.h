#include <stdio.h>

#include "../commands/cdCommand.h"
#include "../commands/killCommand.h"
#include "../commands/psCommand.h"
#include "../commands/pwdCommand.h"
#include "historyHandler.h"

#define MAXARGS 128

struct cmd {
    struct cmd *next;
    int terminator;
    char *exe_path;
    int nargs;
    char *arg[MAXARGS];
};

/**
 * It Handle the user input on Console and pass it to the command 
 * @param c command
 * @return return 1 if the command available, 0 on no command available
 */
int handle(struct cmd *c);
