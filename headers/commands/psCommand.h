#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH_SIZE 1024

void ps(char **args);
char *timeParser(char *ptime, unsigned long time);
char **getStat(char **data, int pid);
void printProc(int pid);