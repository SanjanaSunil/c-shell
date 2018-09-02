#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "prompt.h"
#include "interpret.h"
#include "config.h"
#include "bg.h"

void signal_handler() { return; }

int main() {

    init();

    signal(SIGINT, signal_handler);

    while(1) 
    {
        check_process();
        display_prompt();
        interpret_commands();
    }

    return 0;
}