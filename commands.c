#include "commands.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

int mkdir_(FileSystem *fs, char *pathname){
    // Input validation
    if(strlen(pathname) == 0){
        puts("Error: empty pathname!");
        return 1;
    }

    // Path Spliting
    char *dname, *bname; // bname is for the directory we want to create
    dbname(pathname, dbname, bname);

    // Searching for a node with the parent directory name
    Node *parentDir = searchDir(fs, dbname);
    if(parentDir == NULL || parentDir->type != DIR){
        printf("Error: %s directory does not exists!", dbname);
        return 1;
    }

    Node *curr = parentDir->childPtr;
    while(curr && curr->siblingPtr){ // adding the curr check because the directory could be empty
        if(strcmp(curr->name, bname) == 0 && curr->type == DIR){
            printf("Error: Directory \"%s\" exists!");
            return 1;
        }
        curr = curr->siblingPtr;
    }

    Node *newDirNode = newNode(bname, DIR);

    if(!curr){
        parentDir->childPtr = newDirNode;
    }else {
        curr->siblingPtr = newDirNode; // prev points to the last node at this point.
    }
    newDirNode->parentPtr = parentDir;
    return 0;
}

int rmdir_(FileSystem *fs, char *pathname){
    // input validation
    if(strlen(pathname) == 0){
        puts("Error: empty pathname");
        return 1;
    }

    // path extraction kini
    char *dbName, *bName;
    dbname(pathname, dbName, bName);

    Node *parentDir = searchDir(fs, dbName);
    
    if(!parentDir || parentDir->type == DIR){
        printf("Error: %s, directory not valid", dbName);
        return 1;
    }

    Node *target = parentDir->childPtr, *prev = NULL;
    while( target && strcmp(target->name, bName) != 0){
        prev = target;
        target = target->siblingPtr;
    }

    if(!target || target->type != DIR){
        printf("Error: %s not a valid directory", bName);
        return 1;
    }
   
    if(prev){
        // the directory is not the oldest in the parent directory
        // so we shift pointers skipping the target directory
        prev->siblingPtr = target->siblingPtr;
    }else {
        // the only time where prev is NUll is when the target is the childPtr;
        parentDir->childPtr = target->siblingPtr;
    }

    free(target);
    return 0;

}

int findCmd(char *cmdName, char **commands){
    // allowing the command list to come in the input to avoid the 
    // list being allocated automatically - everytime the function is called.
    for ( int i = 0; i < len(commands); i++){
        if(strcmp(cmdName, commands[i]) == 0){
            return i;
        }
    }
    return -1; // the command not found.
}

void pwd_helper(Node *curr){
    if (!curr) return;
    pwd_helper(curr->parentPtr);
    // adding the if check to avoid printing the 
    // root name -> will result in "//"
    if(curr->parentPtr) printf("%s/", curr->name);
}

int pwd_(Node *cwd){
    if(!cwd) {
        puts("File System Error");
    }
    printf("/");
    pwd_helper(cwd);
    return 1;
}