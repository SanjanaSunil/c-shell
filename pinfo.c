#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

// This function stores the pid into a string
void ConvertPID(char stringpid[],int pid)
{
    int temp = pid;
    int count = 0;
    while(temp != 0)
    {
      count++;
      temp = temp/10;
    }
    temp = pid;
    stringpid[count] = 0;
    while(temp != 0)
    {
      stringpid[count-1] = temp%10 + '0';
      temp = temp/10;
      count--;
    }

    return;
}

void pinfo(char *token)
{
      token = strtok(NULL," \n\r\t");
      int pid;
      char stringpid[1000];
      if(token == NULL)
      {
        pid = getpid();
        ConvertPID(stringpid,pid);
        printf("DBUG %s\n", stringpid);
      }
      else
      {
        strcpy(stringpid,token);
      }
      int source,link;
      char s[1000];
      char p[1000];
      char c[1000000];
      strcpy(s,"/proc/");
      strcpy(p,"/proc/");
      char ExecutePath[1000];
      strcat(s,stringpid);
      strcat(s,"/stat");
      strcat(p,stringpid);
      strcat(p,"/exe");
      source  = open(s,O_RDONLY);
      link = readlink(p,ExecutePath,1000);
      if(source < 0 || link < 0)
      {
        fprintf(stderr,"Invalid process id \n");
        close(source);
        close(link);
        return;
      }
      ExecutePath[link] = 0;
      int byte = read(source,c,1000000);
      int count = 1;
      char Id[100],Status[100],VMemory[100],*content;
      content = strtok(c," \n\r\t");
      while(content != NULL)
      {
        if(count == 1)
        {
          strcpy(Id,content);
        }
        if(count == 3)
        {
          strcpy(Status,content);
        }
        if(count == 25)
        {
          strcpy(VMemory,content);
        }
        count++;
        content = strtok(NULL," \n\r\t");
      }
      printf("pid -- %s\n",Id);
      printf("Process Status -- %s\n",Status);
      printf("Virtual Memory -- %s\n",VMemory);
      printf("Executable Path -- %s\n",ExecutePath);
      close(source);
      close(link);
      return;
}