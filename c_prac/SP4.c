#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


#define READ_LEN 1000

int
main(int argc, char **argv)
{
    int fd[2];
    pipe(fd);

    if (fork() == 0){
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execlp("grep", "grep", "^[_[:alpha:]][_[:alnum:]]*(", NULL);
        exit(127);
    }
    
    close(fd[1]);
    
    int fd_two[2];
    pipe(fd_two);
     
    if (fork() == 0){
        dup2(fd_two[1], 1);
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd_two[1]);
        close(fd_two[0]);
        execlp("cut", "cut", "-d(", "-f1", NULL);
        exit(127);
    }
    
    close(fd[0]);
    close(fd_two[1]);

    
    if (fork() == 0){
        dup2(fd_two[0], 0);
        close(fd_two[0]);
        execlp("sort", "sort", NULL);
    }
   
    close(fd_two[0]);
   
    while (wait(NULL) != -1){}
    return 0;
}

