#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/types.h>

#include "builtin.h"

void execute(char **command) {

    char *token = strtok(*command, " \t\n\r");
    if(token==NULL) return;

    if(strcmp(token, "exit")==0) exit(0);
    else if(strcmp(token, "pwd")==0) pwd();
    else if(strcmp(token, "cd")==0) 
    {
        token = strtok(NULL, " \t");
        cd(token);
    }

    return;
}

void interpret_commands() {

    char *token;
    char *commands[2000];
    char input[2000];
    int i, j, count;

    // scanf("%[^\n]%*c", input);
    fgets(input, sizeof(input), stdin);

    // Separate out the commands with ; as delimiter
    token = strtok(input, ";\n");
    count = 0;

    while(token!=NULL)
    {
        commands[count++] = token; 
        token = strtok(NULL, ";");
    }

    // Execute each command
    for(i=0; i<count; ++i) execute(&commands[i]);

    return;
}