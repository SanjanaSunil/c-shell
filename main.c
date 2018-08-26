#include <stdio.h>
#include <stdlib.h>

#include "prompt.h"
#include "interpret.h"

int main() {

    init();

    while(1) 
    {
        display_prompt();
        interpret_commands();
    }

    return 0;
}