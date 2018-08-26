#include <stdio.h>
#include <stdlib.h>

// For display prompt
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
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    const char *user = pw->pw_name;

    // Get system name
    struct utsname sysinfo;
    uname(&sysinfo);
    const char *sysname = sysinfo.nodename;

    // Get current working dir
    //char *pwd = malloc(1000*sizeof(char));
    char cur_dir[1000];
	getcwd(cur_dir, sizeof(cur_dir));

    // Get home dir
    const char *home_dir = pw->pw_dir;

    // Get current dir in relation to home
    int i;  
    int home_dir_len = strlen(home_dir);
    int cur_dir_len = strlen(cur_dir);
    for(i=0; i<home_dir_len; ++i) if(home_dir[i]!=cur_dir[i]) break;

    // Print command prompt
    printf(BOLD GRN "%s@%s" RESET, user, sysname);
    printf(BOLD WHT ":" RESET);
    if(i!=home_dir_len) printf(BOLD BLU "%s" RESET, cur_dir);
    else printf(BOLD CYN "~%s" RESET, &cur_dir[i]);
    printf(BOLD WHT "> " RESET);

    return;
}

void pwd() {

    char cur_dir[1000];
	getcwd(cur_dir, sizeof(cur_dir));
    printf("%s\n", cur_dir);
    return;
    
}

void execute(char **command) {

    char *token = strtok(*command, " \t\n");
    if(strcmp(token, "exit")==0) exit(0);
    else if(strcmp(token, "pwd")==0) pwd();

    return;
}

int main() {

    char *token;
    char *commands[2000];
    int i, j, count;

    while(1) 
    {
        display_prompt();

        char input[2000];
        // scanf("%[^\n]%*c", input);
        fgets(input, sizeof(input), stdin);

        token = strtok(input, ";");
        count = 0;
        while(token!=NULL)
        {
            //printf("%s\n", token); 
            // execute(&token);
            commands[count++] = token; 
            token = strtok(NULL, ";");
        }

        for(i=0; i<count; ++i) execute(&commands[i]);
    }

    return 0;
}