#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "config.h"

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