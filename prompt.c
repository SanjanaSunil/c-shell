#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/types.h>

#include "styles.h"

char HOME[1000];
int bg_procs[1024];
char *bg_procs_name[1024];

void init() {

	getcwd(HOME, sizeof(HOME));

    int i = 0;
    for(i=0; i<1024; ++i)
    {
        bg_procs[i] = -1;
        bg_procs_name[i] = "Process";
    }
    
    return;
}

void display_prompt() {

    // Get username
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    char *user = pw->pw_name;

    // Get system name
    struct utsname sysinfo;
    uname(&sysinfo);
    char *sysname = sysinfo.nodename;
    
    // Get current working dir
    char cur_dir[1000];
	getcwd(cur_dir, sizeof(cur_dir));

    // Get current dir in relation to home
    int home_len = strlen(HOME);
    int i = 0;
    for(i=0; i<home_len; ++i) if(HOME[i]!=cur_dir[i]) break;

    // Print command prompt
    printf(BOLD WHT "<" RESET);
    printf(BOLD GRN "%s@%s" RESET, user, sysname);
    printf(BOLD WHT ":" RESET);
    if(i!=home_len) printf(BOLD CYN "%s" RESET, cur_dir);
    else printf(BOLD CYN "~%s" RESET, &cur_dir[i]);
    printf(BOLD WHT "> " RESET);

    return;
}