#ifndef commands_h
#define commands_h

#include "file_system.h"
#include <stdio.h>

#define ARR_S 64

/* check if specified command name exists in list of 
available commands.*/
int findCmd(char *cmdName);
int mkdir_(FileSystem *fs, const char *pathname);
int rmdir_(FileSystem *fs, const char *pathname);
int cd_(FileSystem *fs, const char *pathname);
int rm_(FileSystem *fs, const char *pathname);
int creat(FileSystem *fs, const char *pathname);
int ls_(FileSystem *fs, const char *pathname);
int save(FileSystem *fs, const char *filename);
void saveUtil(Node *node, FILE *fptr, const char *prefix);
int reload(FileSystem *fs, const char *filename);
int quit(FileSystem *fs);
int pwd_(Node *cwd);
void pwd_helper(Node *node);
int menu_();


#endif