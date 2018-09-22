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

    char *endchar = token + strlen(token) - 1;
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

void add_env(char *token) {

    token = strtok(NULL, " \t\n\r");
    if(token==NULL) {printf("Usage: setenv var [value]\n"); return;}

    char *var = token;
    char *value = "";

    token = strtok(NULL, " \t\n\r");
    if(token!=NULL) 
    {
        value = token;
        token = strtok(NULL, " \t\n\r");
        if(token!=NULL) {printf("Usage: setenv var [value]\n"); return;}
    }

    if(setenv(var, value, 1)==-1) {perror("Error"); return;}

    return;
}

void remove_env(char *token) {

    token = strtok(NULL, " \t\n\r");
    if(token==NULL) {printf("Usage: unsetenv var\n"); return;}

    char *var = token;

    token = strtok(NULL, " \t\n\r");
    if(token!=NULL) {printf("Usage: unsetenv var\n"); return;}

    if(unsetenv(var)==-1) {perror("Error"); return;}

    return;
}

void jobs(char *token, int kjob) {

    token = strtok(NULL, " \t\n\r");
    if(token!=NULL && !kjob) {printf("Usage: jobs\n"); return;}
    
    int job_id = -1;
    int kjob_pid = -1;
    int signal_no = -1;

    char *temp;
    char *endchar;
    char **endptr;

    if(kjob) 
    {
        if(token==NULL) {printf("Usage: kjob <job number> <signal number>\n"); return;}

        temp = token;
        endchar = temp + strlen(temp) - 1;
        endptr = &endchar;
        job_id = strtol(temp, endptr, 10);
        token = strtok(NULL, " \t\n\r");

        if(token==NULL) {printf("Usage: kjob <job number> <signal number>\n"); return;}

        temp = token;
        endchar = temp + strlen(temp) - 1;
        endptr = &endchar;
        signal_no = strtol(temp, endptr, 10);
    }

    int bg_create_time[1024];
    char *bg_state[1024];
    int bg_pid[1024];
    char *bg_name[1024];

    int i = 0, j = 0;
    for(i=0; i<1023; ++i) bg_create_time[i] = -1;

    for(i=0; i<1023; ++i) 
    {
        if(bg_procs[i]==-1) continue;

        int pid;
        char stringpid[1000];
        pid = bg_procs[i];
        sprintf(stringpid, "%d", pid);

        char proc_stat[1000];
        snprintf(proc_stat, sizeof(proc_stat), "%s%s%s", "/proc/", stringpid, "/stat");

        int stat_fd = open(proc_stat, O_RDONLY);
        if(stat_fd<0) continue;

        char process_info[100000];
        size_t read_stat = read(stat_fd, process_info, sizeof(process_info));
        if(read_stat<0) continue;

        int count = 1;
        char *info = strtok(process_info, " \n\r\t");
        while(info!=NULL)
        {
            if(count==3) bg_state[i] = info;
            if(count==22) 
            {
                endchar = info + strlen(info) - 1;
                endptr = &endchar;
                bg_create_time[i] = strtol(info, endptr, 10);
                break;
            }
            count++;
            info = strtok(NULL, " \n\r\t");
        }

        bg_pid[i] = bg_procs[i];
        bg_name[i] = bg_procs_name[i];

        close(stat_fd);
    }

    // Sorting 
    for(i=0; i<1023; ++i)
    {
        for(j=i+1; j<1023; ++j)
        {
            if(bg_create_time[j]<=bg_create_time[i])
            {
                int temp_time = bg_create_time[i];
                bg_create_time[i] = bg_create_time[j];
                bg_create_time[j] = temp_time;

                char *temp_state = bg_state[i];
                bg_state[i] = bg_state[j];
                bg_state[j] = temp_state;

                int temp_pid = bg_pid[i];
                bg_pid[i] = bg_pid[j];
                bg_pid[j] = temp_pid;

                char *temp_name = bg_name[i];
                bg_name[i] = bg_name[j];
                bg_name[j] = temp_name;
            }
        }
    }

    int count = 1;
    for(i=0; i<1023; ++i)
    {
        if(bg_create_time[i]==-1) continue;

        if(strcmp(bg_state[i], "R")==0) bg_state[i] = "Running";
        else if(strcmp(bg_state[i], "S")==0) bg_state[i] = "Sleeping";
        else if(strcmp(bg_state[i], "D")==0) bg_state[i] = "Sleeping";
        else if(strcmp(bg_state[i], "Z")==0) bg_state[i] = "Zombie";
        else if(strcmp(bg_state[i], "T")==0) bg_state[i] = "Stopped";
        else if(strcmp(bg_state[i], "t")==0) bg_state[i] = "Stopped";
        else if(strcmp(bg_state[i], "W")==0) bg_state[i] = "Paging";
        else if(strcmp(bg_state[i], "X")==0) bg_state[i] = "Dead";
        else if(strcmp(bg_state[i], "x")==0) bg_state[i] = "Dead";
        else if(strcmp(bg_state[i], "K")==0) bg_state[i] = "Wakekill";
        else if(strcmp(bg_state[i], "W")==0) bg_state[i] = "Waking";
        else if(strcmp(bg_state[i], "P")==0) bg_state[i] = "Parked";

        if(!kjob) printf("[%d]   %s   %s [%d]\n", count, bg_state[i], bg_name[i], bg_pid[i]);
        else if(count==job_id) 
        {
            kjob_pid = bg_pid[i]; 
            if(kill(kjob_pid, signal_no)<0) perror("Error");
        }

        count++;
    }

    return;
}