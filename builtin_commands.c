#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/dir.h>  
#include <sys/stat.h>    
#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <time.h>

#include "config.h"
#include "styles.h"

void pwd() {

    char cur_dir[1000];
	getcwd(cur_dir, sizeof(cur_dir));
    printf("%s\n", cur_dir);

    return;
}


void cd(char *dest) {

    dest = strtok(NULL, " \t\n\r");

    if(dest==NULL || (strcmp(dest,"~")==0) || (strcmp(dest,"~/")==0)) chdir(HOME);
    else if((dest[0]=='~') && (dest[1]=='/')) 
    {
        char destination[1000];
        strcpy(destination, HOME);
        strcat(destination, &dest[1]);
        if(chdir(destination)<0) perror("Error");
    }
    else if(chdir(dest)<0) perror("Error");
    
    return;
}


void echo(char *token) {

    token = strtok(NULL, " \t\n\r");

    while(token!=NULL)
    {
        printf("%s ", token); 
        token = strtok(NULL, " \t\n\r");
    }

    printf("\n");
    
    return;
}


void ls(char *token) {

    token = strtok(NULL, " \t\n\r");

    int a_flag = 0;
    int l_flag = 0;

    char *dirs[2000];
    int count = 0;

    int i, j, k;

    while(token!=NULL)
    {  
        if(strcmp(token, "-a")==0) a_flag = 1;
        else if(strcmp(token, "-l")==0) l_flag = 1;
        else if(strcmp(token, "-la")==0 || strcmp(token, "-al")==0) {a_flag = 1; l_flag = 1;}
        else dirs[count++] = token;

        token = strtok(NULL, " \t\n\r");
    }

    if(count==0) dirs[count++] = token;

    // Iterating through the directories on which we do ls
    for(i=0; i<count; ++i) 
    {
        char dir[1000];

        // Check for home
        if(dirs[0]==NULL)
        {   
            char present_dir[1000];
	        getcwd(present_dir, sizeof(present_dir));
            strcpy(dir, present_dir);
        }
        else if((strcmp(dirs[i],"~")==0) || (strcmp(dirs[i],"~/")==0)) strcpy(dir, HOME);
        else if((dirs[i][0]=='~') && (dirs[i][1]=='/')) 
        {
            strcpy(dir, HOME);
            strcat(dir, &(dirs[i])[1]);
        }
        else strcpy(dir, dirs[i]);

        // ls on the current directory
        DIR* cur_dir = opendir(dir);
        struct dirent *files;
        struct stat filestat;

        if(cur_dir)
        {
            if(i>0) printf("\n");

            char buf[512];  

            while((files = readdir(cur_dir)) != NULL)
            {
                sprintf(buf, "%s/%s", dir, files->d_name);
                stat(buf, &filestat);
                if(!l_flag)
                {
                    if(!a_flag)
                    {
                        if((files->d_name[0])!='.') 
                        {
                            if(S_ISDIR(filestat.st_mode)) printf(BOLD BLU "%s  " RESET, files->d_name);
                            else if(filestat.st_mode & S_IXUSR) printf(BOLD GRN "%s  " RESET, files->d_name);
                            else printf("%s  ", files->d_name);
                        }
                    }
                    else 
                    {
                        if(S_ISDIR(filestat.st_mode)) printf(BOLD BLU "%s  " RESET, files->d_name);
                        else if(filestat.st_mode & S_IXUSR) printf(BOLD GRN "%s  " RESET, files->d_name);
                        else printf("%s  ", files->d_name);
                    }
                }
                else 
                {
                    mode_t mode = filestat.st_mode;

                    char filetype = '?';
                    if(S_ISREG(mode)) filetype = '-';
                    if(S_ISLNK(mode)) filetype = 'l';
                    if(S_ISDIR(mode)) filetype = 'd';
                    if(S_ISBLK(mode)) filetype = 'b';
                    if(S_ISCHR(mode)) filetype = 'c';
                    if(S_ISFIFO(mode)) filetype = '|';

                    char ur = '-', uw = '-', ux = '-';
                    char gr = '-', gw = '-', gx = '-';
                    char or = '-', ow = '-', ox = '-';
                    if(mode & S_IRUSR) ur = 'r';
                    if(mode & S_IWUSR) uw = 'w';
                    if(mode & S_IXUSR) ux = 'x';
                    if(mode & S_IRGRP) gr = 'r';
                    if(mode & S_IWGRP) gw = 'w';
                    if(mode & S_IXGRP) gx = 'x';
                    if(mode & S_IROTH) or = 'r';
                    if(mode & S_IWOTH) ow = 'w';
                    if(mode & S_IXOTH) ox = 'x';

                    struct group *grp;
                    struct passwd *usr;
                    uid_t usr_id = filestat.st_uid; 
                    gid_t grp_id = filestat.st_gid;
                    grp = getgrgid(grp_id);
                    usr = getpwuid(usr_id);

                    char date[256];
                    strftime(date, 20, "%b %d %H:%M", localtime(&(filestat.st_ctime)));

                    if(!a_flag)
                    {
                        if((files->d_name[0])!='.') 
                        {
                            printf("%c%c%c%c%c%c%c%c%c%c\t", filetype, ur, uw, ux, gr, gw, gx, or, ow, ox);
                            printf("%s\t%s\t", usr->pw_name, grp->gr_name);
                            printf("%ld\t%s\t", filestat.st_size, date);
                            if(S_ISDIR(filestat.st_mode)) printf(BOLD BLU "%s\n" RESET, files->d_name);
                            else if(filestat.st_mode & S_IXUSR) printf(BOLD GRN "%s\n" RESET, files->d_name);
                            else printf("%s\n", files->d_name);
                        }
                    }
                    else 
                    {
                        printf("%c%c%c%c%c%c%c%c%c%c\t", filetype, ur, uw, ux, gr, gw, gx, or, ow, ox);
                        printf("%s\t%s\t", usr->pw_name, grp->gr_name);
                        printf("%ld\t%s\t", filestat.st_size, date);
                        if(S_ISDIR(filestat.st_mode)) printf(BOLD BLU "%s\n" RESET, files->d_name);
                        else if(filestat.st_mode & S_IXUSR) printf(BOLD GRN "%s\n" RESET, files->d_name);
                        else printf("%s\n", files->d_name);
                    }
                    
                }

            }

            if(!l_flag) printf("\n");

            closedir(cur_dir);

            if(dirs[0]==NULL) return;
        }
        else
        {
            perror("Error");
            continue;
        }
    }

    return;
}