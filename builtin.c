#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/types.h>

void pwd() {

    char cur_dir[1000];
	getcwd(cur_dir, sizeof(cur_dir));
    printf("%s\n", cur_dir);
    
    return;
}

void cd(char *dest) {

    printf("dbug %s\n", dest);

    if(dest==NULL) printf("go to home");
    chdir(dest);

    return;
}