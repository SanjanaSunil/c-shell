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

void ctrlz_handler() { return; }

// static sigjmp_buf env;
// static volatile sig_atomic_t jump_active = 0;

// void sigint_handler(int signo) {
//     if (!jump_active) return;
//     siglongjmp(env, 42);
// }

int main() {

    init();

    // struct sigaction s;
    // s.sa_handler = sigint_handler;
    // sigemptyset(&s.sa_mask);
    // s.sa_flags = SA_RESTART;
    // sigaction(SIGINT, &s, NULL);

    signal(SIGINT, ctrlz_handler);
    signal(SIGTSTP, ctrlz_handler);

    while(1) 
    {
        // if(sigsetjmp(env, 1) == 42) printf("\n");
        // jump_active = 1; 
        check_process();
        display_prompt();
        interpret_commands();
    }

    return 0;
}