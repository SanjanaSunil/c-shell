#include <stdio.h>
#include <stdlib.h>

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

    fprintf(stderr, "Error: Too many processes running in the background\nThe shell will exit now\n");
    exit(0);
    
    return;
}

void remove_bg(int pid) {

    int i = 0;

    for(i=0; i<1023; ++i)
    {
        if(bg_procs[i]==pid) 
        {
            bg_procs[i] = -1;
            return;
        }
    }

    return;
}