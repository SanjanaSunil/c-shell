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

#include "config.h"
#include "styles.h"

volatile sig_atomic_t stop;

void handle_sigint(int signum) { stop = 1; }

void dynamic_clock(char *token) {

    stop = 0;

    signal(SIGINT, handle_sigint);

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

    while(!stop)
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

void pinfo(char *token)
{
    token = strtok(NULL," \n\r\t");

    int pid;
    char stringpid[1000];

    if(token==NULL)
    {
        pid = getpid();
        sprintf(stringpid, "%d", pid);
    }
    else strcpy(stringpid, token);

    // Get process info
    char proc_stat[1000];
    snprintf(proc_stat, sizeof(proc_stat), "%s%s%s", "/proc/", stringpid, "/stat");

    int stat_fd = open(proc_stat, O_RDONLY);
    if(stat_fd<0)
    {
        fprintf(stderr, "Invalid process ID\n");
        return;
    }

    char process_info[100000];
    size_t read_stat = read(stat_fd, process_info, sizeof(process_info));
    if(read_stat<0)
    {
        fprintf(stderr, "Invalid process ID\n");
        return;
    }

    int count = 0;
    char *info = strtok(process_info, " \n\r\t");
    while(info!=NULL)
    {
        if(count==0) printf("pid -- %s\n", info);
        if(count==2) printf("Process Status -- %s\n", info);
        if(count==22) printf("%s {Virtual Memory}\n", info);

        count++;
        info = strtok(NULL, " \n\r\t");
    }

    close(stat_fd);

    // Get executable path
    char proc_exec[1000];  
    snprintf(proc_exec, sizeof(proc_exec), "%s%s%s", "/proc/", stringpid, "/exe");

    char exec_path[1000];
    ssize_t exec_path_len = readlink(proc_exec, exec_path, sizeof(exec_path)-1);
    if(exec_path_len<0)
    {
        fprintf(stderr, "No executable link\n");
        return;
    }
    exec_path[exec_path_len] = '\0';

    int home_len = strlen(HOME);
    int i = 0;
    for(i=0; i<home_len; ++i) if(HOME[i]!=exec_path[i]) break;
    
    printf("Excutable path -- ");
    if(i!=home_len) printf("%s\n", exec_path);
    else printf("~%s\n", &exec_path[i]);

    return;
}

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