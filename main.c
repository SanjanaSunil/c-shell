#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <setjmp.h>

#include "prompt.h"
#include "interpret.h"
#include "config.h"
#include "bg.h"

int main() {

    init();

    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGSTOP, SIG_IGN);

    while(1) 
    {
        check_process();
        display_prompt();
        interpret_commands();
    }

    return 0;
}