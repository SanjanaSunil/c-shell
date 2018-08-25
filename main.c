#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/types.h>

#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"
#define BOLD  "\033[1m"
#define RESET  "\x1B[0m"

void display_prompt() {

    // Get username
    const char *user;
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if(pw) user = pw->pw_name;
    else user = "user";

    // Get system name
    const char *sysname;
    struct utsname sysinfo;
    uname(&sysinfo);
    sysname = sysinfo.nodename;

    // Get current working dir
    char *pwd = malloc(100*sizeof(char));
	getcwd(pwd, 100);

    // Get home dir
    const char *home_dir = pw->pw_dir;

    // Get current dir in relation to home
    int i;  
    int home_dir_len = strlen(home_dir);
    int pwd_len = strlen(pwd);
    for(i=0; i<home_dir_len; ++i) if(home_dir[i]!=pwd[i]) break;

    // Print command prompt
    printf(BOLD GRN "%s@%s" RESET, user, sysname);
    printf(BOLD WHT ":" RESET);
    if(i!=home_dir_len) printf(BOLD BLU "%s" RESET, pwd);
    else printf(BOLD BLU "~%s" RESET, &pwd[i]);
    printf(BOLD WHT "> \n" RESET);

    return;
}

int main() {

    display_prompt();

    return 0;
}