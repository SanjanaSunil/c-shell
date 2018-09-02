#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "config.h"

char *bg_procs_name[1024];
void get_process_name(int, int);

void add_bg(int pid, char *proc_name) {

    int i = 0;

    for(i=0; i<1023; ++i)
    {
        if(bg_procs[i]==-1)
        {
            bg_procs[i] = pid;  
            // get_process_name(i, pid);
            return;
        }
    }

    fprintf(stderr, "Error: Too many processes running in the background\nThe shell will exit now\n");
    exit(0);
    
    return;
}

void get_process_name(int index, int pid) {

    char stringpid[1000];
    sprintf(stringpid, "%d", pid);

    char proc_name[1000];
    snprintf(proc_name, sizeof(proc_name), "%s%s%s", "/proc/", stringpid, "/cmdline");

    int fd = open(proc_name, O_RDONLY);
    if(fd<0) return;

    char command[1000];
    size_t read_stat = read(fd, command, sizeof(command));
    if(read_stat<0) return;

    bg_procs_name[index] = command; 
    return;
}

void check_process() {
    
    pid_t rpid;
    int status;

    for(int i=0; i<1023; ++i)
    {
        if(bg_procs[i]!=-1) 
        {
            rpid = waitpid(bg_procs[i], &status, WNOHANG); 

            // if(WIFEXITED(status)) fprintf(stderr, "Process with pid %d exited normally\n", bg_procs[i]);
            // if(WIFSIGNALED(status)) fprintf(stderr, "Process with pid %d exited because of signal\n", bg_procs[i]);
            // if(WIFSTOPPED(status)) fprintf(stderr, "Process with pid %d was killed\n", bg_procs[i]);

            if(rpid!=0) 
            {
                fprintf(stderr, "pid %d exited normally\n", bg_procs[i]);
                bg_procs[i] = -1;
            }
        }
    }

    return;
}