#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

#include "styles.h"

void remindme(char *token) {

    token = strtok(NULL, " \t\n\r");
    if(token==NULL) 
    {
        printf("\nUsage: remindme [seconds] [message]\n");
        return;
    }

    char *endchar = token + strlen(token)-1;
    char **endptr = &endchar;
    int seconds = strtol(token, endptr, 10);
    // int seconds = atoi(token);

    token = strtok(NULL, " \t\n\r");
    if(token==NULL) 
    {
        printf("\nUsage: remindme [seconds] [message]\n");
        return;
    }

    char *message[2000];
    int count = 0;
    while(token!=NULL) 
    {
        message[count++] = token;
        token = strtok(NULL, " \n\r\t");
    }

    if(seconds>0) sleep(seconds);
    
    printf(BOLD RED "\nREMINDER" RESET ": ");

    int start = 0;
    int end = count-1;

    int i = 0;
    for(i=start; i<=end; ++i) printf("%s ", message[i]);

    printf("\n");

    return;
}