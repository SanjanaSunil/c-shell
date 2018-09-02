#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void system_command(char *token) {

    char *command[2000];
    int count = 0;

    while(token!=NULL)
    {
        command[count++] = token;
        token = strtok(NULL, " \t\n\r");
    }

    if(execvp(command[0], command)<0) 
    {
        fprintf(stderr, "%s: command not found\n", command[0]);
        _exit(0);
    }

    return;
}