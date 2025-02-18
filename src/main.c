#include <stdio.h>
#include <string.h>
#include "file_system.h"
#include "commands.h"

void unixTreeSimulator(FileSystem *fs){
    menu_(); // the user should see available commands first
    int quit_flag = 1;
    while (quit_flag)
    {   
        char line[128]="";
        char command[16] ="", pathName[64]="";
        printf("user@machine");
        pwd_(fs->cwd);
        printf(" %% ");
        if(!fgets(line, sizeof(line), stdin)){
            puts("Error reading command!");
            continue;
        }
        // push the commmand and pathname
        sscanf(line, "%s %s", command, pathName);
        int cmdIndex = findCmd(command);
        if( cmdIndex < 0 || cmdIndex > 10){
            printf("command not found: %s\n", command); // early exit.
        }
        switch(cmdIndex){
            case 0: mkdir_(fs, pathName); break;
            case 1: rmdir_(fs, pathName); break;
            case 2: ls_(fs, pathName); break;
            case 3: cd_(fs, pathName); break;
            case 4: 
                pwd_(fs->cwd); 
                printf("\n"); break;
            case 5: creat(fs, pathName); break;
            case 6: rm_(fs, pathName); break;
            case 7: reload(fs, "DefaultFile.txt"); break;
            case 8: save(fs, "DefaultFile.txt"); break;
            case 9: menu_(); break;
            case 10: quit(fs); quit_flag=0; break;
            default: break;
        }   
    }
    
}

int main(){
    FileSystem fs;
    fs.root = fs.cwd = newNode("/", DIR); // initialize root to cwd.
    unixTreeSimulator(&fs);
    return 0;
}

