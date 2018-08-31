#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

#include "builtin_commands.h"
#include "system_commands.h"
#include "pinfo.h"
#include "bg.h"
#include "config.h"
#include "reminder.h"

void execute(char *command) {

    // Check for bg process
    int i = 0;
    int background = 0;
    for(i=strlen(command)-1;i>=0;--i)
    {
        if(command[i]==' ') continue;
        else if(command[i]=='&')
        {
            background = 1;
            command[i] = '\0';
            break;
        }
        else break;
    }

    char *token = strtok(command, " \t\n\r");
    if(token==NULL) return;

    if(strcmp(token, "exit")==0) _exit(0);
    else if(strcmp(token, "pwd")==0) pwd();
    else if(strcmp(token, "cd")==0) cd(token);
    else if(strcmp(token, "echo")==0) echo(token);
    else if(strcmp(token, "ls")==0) ls(token);
    else if(strcmp(token, "pinfo")==0) pinfo(token);

    if(!strcmp(token, "cd") || !strcmp(token, "pwd") || !strcmp(token, "echo") || !strcmp(token, "ls") || !strcmp(token, "pinfo")) return;

    if(strcmp(token, "remindme")==0) background = 1;

    int status;
    pid_t pid = fork();
    if(pid==0)
    {
        if(strcmp(token, "remindme")==0) 
        {
            remindme(token);
            _exit(0);
        }
        else system_command(token);
    }
    else 
    {
        if(!background) while(wait(&status)!=pid);
        else if(strcmp(token, "remindme")!=0) add_bg(pid);
    }

    return;
}

void interpret_commands() {

    char *token;
    char *commands[2000];
    char input[2000];
    int i, j, count;

    fgets(input, sizeof(input), stdin);
    
    token = strtok(input, ";\n");
    count = 0;

    while(token!=NULL)
    {
        commands[count++] = token; 
        token = strtok(NULL, ";");
    }

    for(i=0; i<count; ++i) execute(commands[i]);

    return;
}