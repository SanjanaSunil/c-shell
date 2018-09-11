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

void add_bg(int pid, char *proc_name) {

    int i = 0;

    for(i=0; i<1023; ++i)
    {
        if(bg_procs[i]==-1)
        {
            bg_procs[i] = pid;
            bg_procs_name[i] = proc_name;  
            return;
        }
    }

    fprintf(stderr, "Error: Too many processes running in the background\nThe shell will exit now\n");
    exit(0);
    
    return;
}

void check_process() {
    
    pid_t rpid;
    int status;

    while((rpid=waitpid(-1, &status, WNOHANG))>0)
    {
        int i;
        for(i=0; i<1023; i++)
        {
            if(bg_procs[i]==rpid)
            {
                bg_procs[i] = -1;
                fprintf(stderr, "%s ", bg_procs_name[i]);
                bg_procs_name[i] = "Process";
                break;
            }
        }

        if(WIFEXITED(status)) fprintf(stderr, "with pid %d exited normally\n", rpid);
        if(WIFSIGNALED(status)) fprintf(stderr, "with pid %d exited because of signal\n", rpid);
        if(WIFSTOPPED(status)) fprintf(stderr, "with pid %d was killed\n", rpid);
    }

    return;
}