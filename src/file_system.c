#include "file_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

Node* newNode(const char *basename, const nodeType type){
    Node* node = (Node*)malloc(sizeof(Node));
    strcpy(node->name, basename);
    node->type = type;
    node->childPtr = node->parentPtr = node->siblingPtr = NULL;
    return node;
}

void dbname(const char *pathName, char *dName, char *bName){
    char temp[128];
    strcpy(temp, pathName);
    strcpy(dName, dirname(temp));
    strcpy(temp, pathName);
    strcpy(bName, basename(temp));
}

Node* searchDir(FileSystem *fs, const char *pathname, const char *cmd){
    // check for cases to just simply return the cwd or root.
    if(!pathname || strcmp(pathname, ".") == 0) return fs->cwd;    // cd new old fds pdf
    if(strcmp(pathname, "/") == 0) return fs->root; // 
    
    // start from current working directory and only switch to root if entered pathname starts with "/".
    Node* curr = fs->cwd;
    if(pathname[0]=='/') curr = fs->root;

    char temp[64], *s;
    strcpy(temp, pathname); // using strtok for tokenization affects the original value

    s = strtok(temp, "/");
    while (s && curr ){
        // move to the oldest child ( file or directory ) in this directory so we can the continue the search by checking it's siblings
        if(curr->childPtr) curr = curr->childPtr;
        while( curr && strcmp(curr->name, s) != 0) curr = curr->siblingPtr;
        // move to the next token
        s = strtok(NULL, "/");
    }

    // the directory is not found if after all the search above it's still pointing to the root.
    if(curr == fs->root) curr = NULL;
    
    if(!curr) printf("%s: %s: Directory does not exist\n", cmd, pathname);
    else if(curr->type != DIR) printf("%s: %s: Not a directory\n", cmd, pathname);
    
    return curr;
}