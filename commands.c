#include "commands.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

const char *commands[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat",
                         "rm", "reload", "save", "menu", "quit"};

int cd_(FileSystem *fs, const char *pathname){
    if (!pathname || strcmp(pathname, "") == 0){
        fs->cwd = fs->root; 
        return 1;
    }
    
    // extra: navigate to the immediate top level directory
    if(strcmp(pathname, "..") == 0){
        // if user at the root directory, just return the root(also the cwd)
        if (fs->cwd->parentPtr) {
            fs->cwd = fs->cwd->parentPtr;
        }
        return 0;
    }

    char dname[ARR_S], bname[ARR_S]; 
    dbname(pathname, dname, bname);

    Node *parentDir = searchNode(fs, dname);
    if(!parentDir){
        printf("cd: %s: Directory does not exist\n", dname);
        return 1;
    }

    Node *pointer = NULL;

    // base directory could be empty
    if(strcmp(bname, "") != 0 && strcmp(bname, "/") != 0){
        pointer = parentDir->childPtr;
        while(pointer && strcmp(pointer->name, bname) != 0){
            pointer = pointer->siblingPtr;
        }

        if(!pointer){
            printf("cd: %s: Directory does not exist\n", bname);
            return 1;
        }

        // only return this error if 
        if(pointer->type != DIR && !pointer->siblingPtr){
            printf("cd: %s: Not a directory\n", bname);
            return 1;
        }

        // handle case where a directory and a file have the same name
        // and the file is created first - the pointer stops at the file since
        // they have the same and exits the while loop.
        while(pointer && strcmp(pointer->name, bname) != 0) pointer = pointer->siblingPtr; 
    }

    fs->cwd = pointer? pointer:parentDir;
    return 0;
}

int mkdir_(FileSystem *fs, const char *pathname){
    // Input validation
    if(strlen(pathname) == 0){
        puts("usage: mkdir <pathname>");
        return 1;
    }

    // Path Splitting
    char dname[ARR_S], bname[ARR_S]; 
    dbname(pathname, dname, bname);

    Node *parentDir = searchNode(fs, dname);
    if(parentDir == NULL){
        printf("mkdir: %s: No such file or directory\n", dname);
        return 1;
    }
    if(parentDir->type != DIR){
        printf("mkdir: %s: Not a directory\n", dname);
        return 1;
    }

    Node *curr = parentDir->childPtr, *prev = NULL;
    while(curr){ 
        if(strcmp(curr->name, bname) == 0 && curr->type == DIR){
            printf("mkdir: %s: Directory exists\n", bname);
            return 1;
        }
        prev = curr;
        curr = curr->siblingPtr;
    }

    Node *newDirNode = newNode(bname, DIR);

    if(!prev){
        parentDir->childPtr = newDirNode;
    }else {
        prev->siblingPtr = newDirNode; // prev points to the last node at this point.
    }
    newDirNode->parentPtr = parentDir;
    return 0;
}

int creat(FileSystem *fs, const char *pathname){
    // same logic as mkdir except node type is _FILE
    if(strlen(pathname) == 0){
        puts("usage: create <pathname>");
        return 1;
    }

    char dname[ARR_S], bname[ARR_S]; 
    dbname(pathname, dname, bname);

    Node *parentDir = searchNode(fs, dname);
    if(parentDir == NULL){
        printf("create: %s: No such file or directory\n", dname);
        return 1;
    }
    if(parentDir->type != DIR){
        printf("create: %s: Not a directory\n", dname);
        return 1;
    }

    Node *curr = parentDir->childPtr, *prev = NULL;
    while(curr){ 
        if(strcmp(curr->name, bname) == 0 && curr->type == _FILE){
            printf("mkdir: %s: Directory exists\n", bname);
            return 1;
        }
        prev = curr;
        curr = curr->siblingPtr;
    }

    Node *newDirNode = newNode(bname, _FILE);

    if(!prev){
        parentDir->childPtr = newDirNode;
    }else {
        prev->siblingPtr = newDirNode; // prev points to the last node at this point.
    }
    newDirNode->parentPtr = parentDir;
    return 0;
}

int rmdir_(FileSystem *fs, const char *pathname){
    // input validation
    if(strlen(pathname) == 0){
        puts("usage: rmdir <pathname>");
        return 1;
    }

    char dname[ARR_S], bname[ARR_S];
    dbname(pathname, dname, bname);

    Node *parentDir = searchNode(fs, dname);
    
    if(parentDir == NULL){
        printf("rmdir: %s: No such file or directory\n", dname);
        return 1;
    }
    if(parentDir->type != DIR){
        printf("rmdir: %s: Not a directory\n", dname);
        return 1;
    }

    Node *target = parentDir->childPtr, *prev = NULL;
    while (target) {
        if (strcmp(target->name, bname) == 0) {
            // we could possibly encounter a file with that name
            if (target->type != DIR) {
                // we have checked all the nodes
                if (!target->siblingPtr) {
                    printf("rmdir: %s: Not a directory\n", bname);
                    return 1; 
                }
                // just skip and continue because we might still have a 
                // file with that name
                prev = target;
                target = target->siblingPtr;
                continue;
            }
            break; // File found
        }
        prev = target;
        target = target->siblingPtr;
    }

    if(!target){
        printf("rmdir: %s: No such file or directory\n", bname);
        return 1;
    }
    
    // Directory must be empty before deleted
    if(target->childPtr){
        printf("rmdir: %s: Cannot remove non-empty directory\n", bname);
        return 1;
    }

    if(prev){
        // the directory is not the oldest in the parent directory
        // so we shift pointers skipping the target directory
        prev->siblingPtr = target->siblingPtr;
    }else {
        // the only time where prev is NULL is when the target is the childPtr
        parentDir->childPtr = target->siblingPtr;
    }

    free(target);
    return 0;
}


int rm_(FileSystem *fs, const char *pathname){
    // Same as rmdir except we check if node is a file
    // We don't check for emptyness since this is file
    if(strlen(pathname) == 0){
        puts("usage: rm <pathname>");
        return 1;
    }

    char dname[ARR_S], bname[ARR_S];
    dbname(pathname, dname, bname);

    Node *parentDir = searchNode(fs, dname);
    
    if(parentDir == NULL){
        printf("rm: %s: No such file or directory\n", dname);
        return 1;
    }
    if(parentDir->type != DIR){
        printf("rm: %s: Not a directory\n", dname);
        return 1;
    }

    Node *target = parentDir->childPtr, *prev = NULL;
    while (target) {
        if (strcmp(target->name, bname) == 0) {
            // we could possibly encounter a directory with that name
            if (target->type != _FILE) {
                // we have checked all the nodes
                if (!target->siblingPtr) {
                    printf("rm: %s: Not a file\n", bname);
                    return 1; 
                }
                // just skip and continue because we might still have a 
                // file with that name
                prev = target;
                target = target->siblingPtr;
                continue;
            }
            break; // File found
        }
        prev = target;
        target = target->siblingPtr;
    }

    if(!target){
        printf("rm: %s: No such file or directory\n", bname);
        return 1;
    }
    
    if(prev){
        prev->siblingPtr = target->siblingPtr;
    }else {
        parentDir->childPtr = target->siblingPtr;
    }

    free(target);
    return 0;
}

int save(FileSystem *fs, const char *filename){
    if(!fs->root){
        puts("File System not initialised!");
        return 1;
    }

    FILE *fptr = fopen(filename, "w"); // open file in write mode
    if(!fptr){
        printf("Unable to open file: %s\n", filename);
        return 1;
    }

    // now we've opened the file, what next?
    fprintf(fptr, "%s\t\t%s\n", fs->root->type == _FILE? "F": "D", "/");
    saveUtil(fs->root->childPtr, fptr, "");
    fclose(fptr);
    return 0;
}

void saveUtil(Node *node, FILE *fptr, const char *prefix){
    if(!node){
        return;
    }

    char tempname[128];
    strcpy(tempname, prefix);
    strcat(tempname, "/");
    strcat(tempname, node->name);

    // preorder traversal
    fprintf(fptr, "%s\t\t%s\n", node->type == _FILE? "F": "D", tempname); 
    saveUtil(node->childPtr, fptr, tempname);
    saveUtil(node->siblingPtr, fptr, prefix);
}

int ls_(FileSystem *fs, const char *pathname){

    char dname[ARR_S], bname[ARR_S];
    dbname(pathname, dname, bname);

    Node *parentDir = searchNode(fs, dname);
    if(!parentDir){
        printf("ls: %s: No such file or directory\n", dname);
        return 1;
    }
    if(parentDir->type != DIR){
        printf("ls: %s: Not a directory\n", dname);
        return 1;
    }
    
    Node *pointer = parentDir;
    if(strcmp(bname, ".") != 0 && strcmp(bname, "/") != 0){
        pointer = pointer->childPtr;
        while(pointer && strcmp(pointer->name, bname) != 0) pointer = pointer->siblingPtr;

        if(!pointer){
            printf("ls: %s: No such file or directory\n", bname);
            return 1;
        }
    }

    if(pointer->type == DIR){
        Node *t = pointer->childPtr;
        while(t){
            printf("%s\t", t->name);
            t = t->siblingPtr;
        } 
        printf("\n");
    }else {
        printf("%s\t\n", pointer->name);
    }

    return 0;
}

int reload(FileSystem *fs, const char *filename){
    FILE *fp = fopen(filename, "r");
    if(!fp){
        printf("Unable to open file: %s\n", filename);
        return 1;
    }

    // read the first line away ( the fs would already be initialized )
    char typename, pathname[128], line[150];  
    fgets(line, sizeof(line), fp);
    while(fgets(line, sizeof(line), fp)){
        // Remove trailing newline character
        line[strcspn(line, "\n")] = 0;
        if (sscanf(line, "%c %s", &typename, pathname) == 2) {
            if(typename == 'D'){
                mkdir_(fs, pathname);
            }else{
                creat(fs, pathname);
            }
        } 
        else{
            printf("Unable to read line in file: %s\n", filename);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

int findCmd(const char *cmdName){
    int len_cmds = sizeof(commands) / sizeof(commands[0]);
    for( int i = 0; i < len_cmds; i++){
        if(strcmp(cmdName, commands[i]) == 0){
            return i;
        }
    }
    return -1; // the command not found.
}

int quit(FileSystem *fs){
    save(fs, "DefaultFile.txt");
    return 0;
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

int menu_() {
    
    printf("Command List\n");
    printf("------------\n");
    // just copy the commands specification from the textbook.
    printf("mkdir pathname :make a new directory for a given pathname\n");
    printf("rmdir pathname :remove the directory, if it is empty\n");
    printf("cd [pathname]  :change CWD to pathname, or to / if no pathname\n");
    printf("ls [pathname]  :list the directory contents of pathname or CWD\n");
    printf("pwd            :print the (absolute) pathname of CWD\n");
    printf("creat pathname :create a FILE node\n");
    printf("rm pathname    :remove the FILE node\n");
    printf("save filename  :save the current file system tree as a file\n");
    printf("reload filename:construct a file system tree from a file\n");
    printf("menu           :show a menu of valid commands\n");
    printf("quit           :save the file system tree, then terminate the program\n");
    
    return 1;
}