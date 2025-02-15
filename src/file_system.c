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

Node* searchDir(const FileSystem *fs, const char *pathname, const char *cmd){
    // check for cases to just simply return the cwd or root.
    if(!pathname || strcmp(pathname, ".") == 0) return fs->cwd;
    if(strcmp(pathname, "/") == 0) return fs->root; // 
    printf("pathanem: %s",pathname);
    // start from current working directory and only switch to root if entered pathname starts with "/".
    Node* curr = fs->cwd;
    if(pathname[0]=='/') curr = fs->root;

    char temp[64];
    strcpy(temp, pathname); // using strtok for tokenization affects the original value

    char *s = strtok(temp, "/");
    while (s && curr) {
        curr = curr->childPtr;  // Move to first child
        while (curr) {
            if (strcmp(curr->name, s) == 0) {
                if(curr->type ==  DIR) break;
                if (curr->type != DIR && !curr->siblingPtr) {
                        printf("%s: %s: Not a directory\n", cmd, pathname);
                        return NULL;  
                } 
            }
            curr = curr->siblingPtr;  // Move to next sibling
        }
        s = strtok(NULL, "/");  // Move to next token
    }
    
    if(!curr) printf("%s: %s: Directory does not exist\n", cmd, pathname);
    
    return curr;
}