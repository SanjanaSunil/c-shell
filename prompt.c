#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/types.h>

#include "styles.h"

const char *USER;
const char *SYSNAME;
char HOME[1000];

void init() {

    // Get username
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    USER = pw->pw_name;

    // Get system name
    struct utsname sysinfo;
    uname(&sysinfo);
    SYSNAME = sysinfo.nodename;

    // Get home dir
	getcwd(HOME, sizeof(HOME));
    // HOME = pw->pw_dir;

    return;
}

void display_prompt() {

    // Get current working dir
    char cur_dir[1000];
	getcwd(cur_dir, sizeof(cur_dir));

    // Get current dir in relation to home
    int home_len = strlen(HOME);
    int i = 0;
    for(i=0; i<home_len; ++i) if(HOME[i]!=cur_dir[i]) break;

    // Print command prompt
    printf(BOLD GRN "%s@%s" RESET, USER, SYSNAME);
    printf(BOLD WHT ":" RESET);
    // if(i!=home_len) printf(BOLD CYN "%s" RESET, cur_dir);
    if(i!=home_len) printf(BOLD CYN "home/%s" RESET, USER);
    else printf(BOLD CYN "~%s" RESET, &cur_dir[i]);
    printf(BOLD WHT "> " RESET);

    return;
}