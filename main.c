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
#define RESET  "\x1B[0m"

void display_prompt() {

    // Username
    const char *user;
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if(pw) user = pw->pw_name;
    else user = "user";

    // System name
    const char *sysname;
    struct utsname sysinfo;
    uname(&sysinfo);
    sysname = sysinfo.nodename;

    // Current working directory
    char *pwd = malloc(100*sizeof(char));
	getcwd(pwd, 100);

    // Home directory
    const char *home_dir = pw->pw_dir;

    int i;
    for(i=0; i<strlen(home_dir); ++i)
    {
        if(home_dir[i] != pwd[i]) break;
    }

    printf(GRN "%s@%s" RESET, user, sysname);
    printf(WHT ":" RESET);
    printf(CYN "~%s" RESET, &pwd[i]);
    printf(WHT ">\n" RESET);

    return;
}

int main() {

    display_prompt();

    return 0;
}