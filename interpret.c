#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "builtin_commands.h"
#include "system_commands.h"
#include "pinfo.h"
#include "bg.h"
#include "config.h"
#include "reminder.h"
#include "clock_cmd.h"

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

    if(strcmp(token, "exit")==0 || strcmp(token, "quit")==0) exit(0);
    else if(strcmp(token, "pwd")==0) pwd();
    else if(strcmp(token, "cd")==0) cd(token);
    else if(strcmp(token, "echo")==0) echo(token);
    else if(strcmp(token, "ls")==0) ls(token);
    else if(strcmp(token, "pinfo")==0) pinfo(token);
    else if(strcmp(token, "clock")==0) dynamic_clock(token);
    else
    {
        if(strcmp(token, "remindme")==0) background = 1;
        int status;
        pid_t pid = fork();
        if(pid==0)
        {
            if(strcmp(token, "remindme")==0) { remindme(token); _exit(0); }
            else system_command(token);
        }
        else 
        {
            if(!background) while(wait(&status)!=pid);
            else if(strcmp(token, "remindme")!=0) add_bg(pid, token);
        }
    }

    return;
}

void parse(char *command) {

    if(strchr(command, '<')!=NULL || strchr(command, '>')!=NULL)
    {
        char *command_words[2000];
        int count = 0;

        char *token = strtok(command, " \t\n\r");
        if(token==NULL) return;

        while(token!=NULL)
        {
            command_words[count++] = token;
            token = strtok(NULL, " \t\n\r");
        }

        int i = 0;
        for(i=0; i<count; ++i)
        {
            if(strcmp(command_words[i], ">")==0 || strcmp(command_words[i], ">>")==0 || strcmp(command_words[i], "<")==0)
            {
                if(i==count-1) {fprintf(stderr, "syntax error near unexpected token `newline'"); return;}

                int j = 0;
                char exec_command[2000];
                exec_command[0] = '\0';

                for(j=0; j<i; ++j)
                {
                    strcat(exec_command, command_words[j]);
                    strcat(exec_command, " ");
                } 
                
                int fd;
                if(strcmp(command_words[i], ">")==0) fd = open(command_words[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                else if(strcmp(command_words[i], ">>")==0) fd = open(command_words[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                else if(strcmp(command_words[i], "<")==0) fd = open(command_words[i+1], O_RDONLY);

                if(fd==-1) {perror("Error"); continue;}

                if(strcmp(command_words[i], ">")==0 || strcmp(command_words[i], ">>")==0)
                {
                    int std_out = dup(1);
                    if(dup2(fd, 1)==-1) perror("Error");
                    close(fd);
                    execute(exec_command);
                    if(dup2(std_out, 1)==-1) perror("Error");
                }
                else if(strcmp(command_words[i], "<")==0)
                {
                    int std_in = dup(0);
                    if(dup2(fd, 0)==-1) perror("Error");
                    close(fd);
                    execute(exec_command);
                    if(dup2(std_in, 0)==-1) perror("Error");
                }
            }
        }
    }
    else execute(command);

    return;
}

void interpret_commands() {

    char *token;
    char *commands[2000];
    char *input = '\0';
    size_t bufr_size_m = 0;
    int i, j, count;

    // char input[2000];
    // fgets(input, sizeof(input), stdin);

    getline(&input, &bufr_size_m, stdin);

    token = strtok(input, ";\n");
    count = 0;

    while(token!=NULL)
    {
        commands[count++] = token; 
        token = strtok(NULL, ";");
    }

    for(i=0; i<count; ++i) parse(commands[i]);

    return;
}