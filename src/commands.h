#ifndef commands_h
#define commands_h

#include "file_system.h"
#include <stdio.h>

#define ARR_S 64

// user commands
int mkdir_(FileSystem *fs, const char *pathname);
int rmdir_(FileSystem *fs, const char *pathname);
int cd_(FileSystem *fs, const char *pathname);
int rm_(FileSystem *fs, const char *pathname);
int creat(FileSystem *fs, const char *pathname);
int ls_(FileSystem *fs, const char *pathname);
int save(FileSystem *fs, const char *filename);
int reload(FileSystem *fs, const char *filename);
int quit(FileSystem *fs);
int pwd_(Node *cwd);
int menu_();

// helper functions
void pwd_helper(Node *node);
void saveUtil(Node *node, FILE *fptr, const char *prefix);
int findCmd(const char *cmdName);
int validateAndExtractPaths(const char *pathname, char *dname, char *bname, const char *cmdName);

#endif