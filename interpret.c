#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "builtin_commands.h"
#include "system_commands.h"

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

    if(strcmp(token, "exit")==0) exit(0);

    pid_t pid = 0; 
    if(background) pid = fork();
    if(pid==0)
    {
        if(strcmp(token, "pwd")==0) pwd();
        else if(strcmp(token, "cd")==0) cd(token);
        else if(strcmp(token, "echo")==0) echo(token);
        else if(strcmp(token, "ls")==0) ls(token);
        else fg(token);
    }
    else if(!background) wait(NULL);

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