#include <signal.h>

#include "config.h"

void signal_handler(int signum) {

    if(signum==2) 
    {
        for(int i=0; i<1023; ++i) if(current_pid==bg_procs[i]) return;
        if(current_pid!=shell_pid) kill(current_pid, 2);
    }
    else if(signum==20)
    {
        for(int i=0; i<1023; ++i) if(current_pid==bg_procs[i]) return;
        if(current_pid!=shell_pid) kill(current_pid, 19);
        return;
    }

    return;
}