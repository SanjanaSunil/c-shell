#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <dirent.h>

#include "system_details.h"

void pwd() {

    char cur_dir[1000];
	getcwd(cur_dir, sizeof(cur_dir));
    printf("%s\n", cur_dir);

    return;
}

void cd(char *dest) {

    dest = strtok(NULL, " \t\n\r");

    if(dest==NULL || (strcmp(dest,"~")==0) || (strcmp(dest,"~/")==0)) chdir(HOME);
    else if((dest[0]=='~') && (dest[1]=='/')) 
    {
        char destination[1000];
        strcpy(destination, HOME);
        strcat(destination, &dest[1]);
        if(chdir(destination)<0) perror("Error");
    }
    else if(chdir(dest)<0) perror("Error");
    
    return;
}

void echo(char *token) {

    token = strtok(NULL, " \t\n\r");

    while(token!=NULL)
    {
        printf("%s ", token); 
        token = strtok(NULL, " \t\n\r");
    }

    printf("\n");
    
    return;
}

void ls(char *token) {

    return;
}