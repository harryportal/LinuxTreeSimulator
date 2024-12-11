#ifndef commands_h
#define commands_h

#include "file_system.h"
#include <stdio.h>

char *commands[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm", "reload", "save", "menu", "quit", NULL};
/* check if specified command name exists in list of 
available commands.*/
int findCmd(char *cmdName, char **commands);

int mkdir_(FileSystem *fs, char *pathname);
int rmdir_(FileSystem *fs, char *pathname);
int pwd_(Node *cwd);

#endif