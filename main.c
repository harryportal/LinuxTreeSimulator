#include <stdio.h>
#include <string.h>
#include "file_system.h"
#include "commands.h"

#define LINE_SIZE 128
#define COMMAND_SIZE 16
#define PATHNAME 64

extern char *commands[]; 
void unixTreeSimulator(FileSystem *fs){
    // declare the varaibles
    char line[LINE_SIZE] = "";
    char command[COMMAND_SIZE] = "", pathName[PATHNAME] = "";

    int quit_flag = 1;
    while (quit_flag)
    {
        printf("user@machine ");
        pwd_(fs);
        printf(" %% ");
        if(!fgets(line, strlen(line), stdin)){
            puts("Error reading command!");
            continue;
        }

        // push the commmand and pathname
        sscanf(line, "%s %s", command, pathName);
        int cmdIndex = findCmd(command, commands);
        switch(cmdIndex){
            case 0: rmdir_(fs, pathName); break;
            case 1: mkdir_(fs, pathName); break;
            default: quit_flag = 0; break;
        }
        
    }
    
}

int main(){
    FileSystem *fs;
    initialize(fs);
    unixTreeSimulator(fs);
    return 0;
}

