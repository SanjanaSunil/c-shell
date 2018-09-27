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
#include <signal.h>

#include "builtin_commands.h"
#include "system_commands.h"
#include "user_commands.h"
#include "bg.h"
#include "config.h"

pid_t shell_pid;
pid_t current_pid;

void ctrlc_handler(int signum) { 
    for(int i=0; i<1023; ++i) if(current_pid==bg_procs[i]) {return;}
    if(current_pid!=shell_pid) kill(current_pid, 9);
    return;
}

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
    else if(strcmp(token, "setenv")==0) add_env(token);
    else if(strcmp(token, "unsetenv")==0) remove_env(token);
    else if(strcmp(token, "jobs")==0) jobs(token, "jobs");
    else if(strcmp(token, "kjob")==0) jobs(token, "kjob");
    else if(strcmp(token, "fg")==0) jobs(token, "fg");
    else if(strcmp(token, "bg")==0) jobs(token, "bg");
    else if(strcmp(token, "overkill")==0) overkill(token);
    else if(strcmp(token, "clock")==0) dynamic_clock(token);
    else
    {
        if(strcmp(token, "remindme")==0) background = 1;
        int status;
        shell_pid = getpid();
        pid_t pid = fork();
        if(pid==0)
        {
            signal(SIGINT, ctrlc_handler);
            current_pid = pid;
            if(strcmp(token, "remindme")==0) {remindme(token); _exit(0);}
            else system_command(token);
        }
        else 
        {
            signal(SIGINT, ctrlc_handler);
            current_pid = pid;
            if(background) setpgid(pid, pid);
            if(!background) while(wait(&status)!=current_pid);
            else /*if(strcmp(token, "remindme")!=0)*/ add_bg(pid, token);
        }
    }

    return;
}

void redirect(char *command) {

    if(strchr(command, '<')!=NULL || strchr(command, '>')!=NULL)
    {
        int input_flag = 0;
        int output_flag = 0;

        if(strchr(command, '<')!=NULL) input_flag = 1;
        if(strchr(command, '>')!=NULL) output_flag = 1;

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
        int fd;
        int std_in = dup(0), std_out = dup(1);
        char exec_command[2000];
        exec_command[0] = '\0';

        if(input_flag)
        {
            for(i=0; i<count; ++i)
            {
                if(strcmp(command_words[i], "<")==0)
                {
                    if(i==count-1) {fprintf(stderr, "syntax error near unexpected token `newline'\n"); return;}

                    fd = open(command_words[i+1], O_RDONLY);
                    std_in = dup(0);
                    if(dup2(fd, 0)==-1) {perror("Error"); break;}
                    close(fd);
                    break;
                }

                strcat(exec_command, command_words[i]);
                strcat(exec_command, " ");
            }
        }

        for(; i<count; ++i)
        {
            if(strcmp(command_words[i], ">")==0) fd = open(command_words[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(strcmp(command_words[i], ">>")==0) fd = open(command_words[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if(strcmp(command_words[i], ">")==0 || strcmp(command_words[i], ">>")==0)
            {
                if(i==count-1) {fprintf(stderr, "syntax error near unexpected token `newline'\n"); return;}

                std_out = dup(1);
                if(dup2(fd, 1)==-1) {perror("Error"); break;}
                close(fd);
                break;
            }
            if(output_flag && !input_flag) 
            {
                strcat(exec_command, command_words[i]);
                strcat(exec_command, " ");
            }
        }   
        
        execute(exec_command);
        if(dup2(std_in, 0)==-1) {perror("Error"); return;}
        close(std_in);
        if(dup2(std_out, 1)==-1) {perror("Error"); return;}
        close(std_out);

        close(fd);
    }
    else execute(command);
    return;
 }

void pipe_check(char *command) {

    char *pipe_command[2000];

    char *token = strtok(command, "|");
    if(token==NULL) return;

    int count = 0;
    while(token!=NULL)
    {
        pipe_command[count++] = token;
        token = strtok(NULL, "|");
    }

    int i = 0;

    int std_in = dup(0), std_out = dup(1);   
    int temp_read = dup(0), temp_write = dup(1);

    for(i=0; i<count; ++i)
    {
        if(dup2(temp_read, 0)==-1) {perror("Error"); break;}
        close(temp_read);

        if(i==count-1) temp_write = dup(std_out);
        else 
        {
            int pipefd[2];
            if(pipe(pipefd)==-1) {perror("pipe"); break;}
            temp_write = pipefd[1];
            temp_read = pipefd[0];
        }

        if(dup2(temp_write, 1)==-1) {perror("Error"); break;}
        close(temp_write);

        redirect(pipe_command[i]); 

    }

    if(dup2(std_in, 0)==-1) {perror("Error"); return;}
    close(std_in);
    if(dup2(std_out, 1)==-1) {perror("Error"); return;}
    close(std_out);

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

    for(i=0; i<count; ++i) pipe_check(commands[i]);

    return;
}