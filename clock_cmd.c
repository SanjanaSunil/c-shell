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
#include <signal.h>

void dynamic_clock(char *token) {

    token = strtok(NULL, " \n\r\t");
    if(token==NULL || strcmp(token, "-t")!=0) 
    {
        printf("Usage: clock -t [interval]\n");
        return;
    }

    token = strtok(NULL, " \n\r\t");

    if(token==NULL) 
    {
        printf("Usage: clock -t [interval]\n");
        return;
    }

    char *endchar = token + strlen(token)-1;
    char **endptr = &endchar;
    int interval = strtol(token, endptr, 10);

    if(interval==0) 
    {
        printf("Usage: clock -t [interval]\n");
        return;
    }

    while(1)
    {
        int fd = open("/proc/driver/rtc", O_RDONLY);
        if(fd<0)
        {
            fprintf(stderr, "Error in getting time\n");
            return;
        }

        char rtc_info[1000];

        read(fd, rtc_info, sizeof(rtc_info));

        char *rtc_time = strtok(rtc_info, " \t\r\n");
        rtc_time = strtok(NULL, " \t\r\n");
        rtc_time = strtok(NULL, " \t\r\n");

        char *rtc_date = strtok(NULL, " \t\r\n");
        rtc_date = strtok(NULL, " \t\r\n");
        rtc_date = strtok(NULL, " \t\r\n");

        printf("%s\t%s\n", rtc_date, rtc_time);

        sleep(interval);

        close(fd);
    }

    return;
}