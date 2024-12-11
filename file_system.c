#include "file_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

Node* newNode(const char *basename, const nodeType type){
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->name, basename);
    newNode->type = type;
    newNode->childPtr = newNode->parentPtr = newNode->siblingPtr;
}

void initialize(FileSystem *fs){
    fs->root = fs->cwd = newNode("/", DIR);
}

void dbName(char *pathName, char *dName, char *bName){
    char *temp;
    strcpy(temp, pathName);
    strcpy(dName, dirname(temp));
    strcpy(temp, pathName);
    strcpy(bName, basename(temp));
}

Node* searchDir(FileSystem *fs, char *pathname){
    // check for cases to just simply return the cwd or root.
    if(!pathname || !strcmp(pathname, ".")) return fs->cwd;
    if(strcmp(pathname, "/")) return fs->root;
    
    // where do we start the search from? cwd or root?
    Node* curr = fs->cwd;
    if(pathname[0]=='/') curr = fs->root;

     // now let the search begin ah ah
    char *temp, *s;
    strcpy(temp, pathname); // using strtok for tokenization affects the original value

    s = strtok(temp, "/");

    while (s && curr ){
        // move to the oldest child ( file or directory ) in this directory so we can the continue the search by checking it's siblings
        if(curr->childPtr) curr = curr->childPtr;
        while( curr && strcmp(curr->name, s) != 0) curr = curr->siblingPtr;

        // move to the next token
        s = strtok(NULL, "/");
    }

    return curr;

}