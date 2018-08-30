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

void add_bg(int pid) {

    int i = 0;

    for(i=0; i<1023; ++i)
    {
        if(bg_procs[i]==-1)
        {
            bg_procs[i] = pid;
            return;
        }
    }

    // if()
    // {
    //     fprintf(stderr, "Error: Too many processes running in the background\nThe shell will exit now\n");
    //     exit(0);
    // }
    
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
            if(rpid!=0) 
            {
                fprintf(stderr, "PID %d exited normally\n", bg_procs[i]);
                bg_procs[i] = -1;
            }
        }
    }

    return;
}