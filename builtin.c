#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/types.h>

#include "prompt.h"

void pwd() {

    char cur_dir[1000];
	getcwd(cur_dir, sizeof(cur_dir));

    int home_len = strlen(HOME);
    int i = 0;
    for(i=0; i<home_len; ++i) if(HOME[i]!=cur_dir[i]) break;

    if(i!=home_len) printf("home/%s\n", USER);
    else printf("home/%s%s\n", USER, &cur_dir[i]);
    
    return;
}

void cd(char *dest) {

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