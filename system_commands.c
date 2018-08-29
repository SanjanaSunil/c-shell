#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void fg(char *token) {

    char *command[2000];
    int count = 0;

    while(token!=NULL)
    {
        command[count++] = token;
        token = strtok(NULL, " \t\n\r");
    }

    pid_t child_pid;
    child_pid = fork();
    if(child_pid==0) 
    {
        execvp(command[0], command);
        fprintf(stderr, "%s: command not found\n", command[0]);
    }
    else
    {
        wait(NULL);
    }

    return;
}

void bg(char *token, int background) {

    char *command[2000];
    int count = 0;

    while(token!=NULL)
    {
        command[count++] = token;
        token = strtok(NULL, " \t\n\r");
    }

    pid_t child_pid;
    child_pid = fork();
    if(child_pid==0) 
    {
        execvp(command[0], command);
        fprintf(stderr, "%s: command not found\n", command[0]);
    }
    else
    {
        if(background==0) wait(NULL);
    }

    return;
}