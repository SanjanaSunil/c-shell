#include <stdio.h>
#include <stdlib.h>

#include "prompt.h"
#include "interpret.h"
#include "config.h"
#include "bg.h"

int main() {

    init();

    while(1) 
    {
        check_process();
        display_prompt();
        interpret_commands();
    }

    return 0;
}