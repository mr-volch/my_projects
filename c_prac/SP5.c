#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


#define READ_LEN 1000

int
main(int argc, char **argv)
{
    int fd[2];
    pipe(fd);
    
    for (int i = 2; i < argc; i++){
        if (fork() == 0){
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            execlp("grep", "grep", argv[1], argv[i], NULL);
            exit(127);
        }
    }
    
    
    close(fd[1]);
    
    int fd_spec[2];
    pipe(fd_spec);
    
    if (fork() == 0){
        dup2(fd_spec[1], 1);
        dup2(fd[0], 0);
        close(fd_spec[0]);
        close(fd_spec[1]);
        close(fd[0]);
        execlp("wc", "wc", NULL);
    }
    
    close(fd_spec[1]);
    close(fd[0]);
    char buf[READ_LEN + 1];
    int n = read(fd_spec[0], buf, READ_LEN);
    close(fd_spec[0]);
    buf[n] = '\0';
    
    int flag = 0;
    for (int i = 0; i < n; i++){
        if (isdigit(buf[i])){
            flag = i;
        }
        if ((flag != 0) && (buf[i] == ' ')){
            buf[i] = '\0';
            break;
        }
    }
    
    printf("%d\n", atoi(buf + flag));    
    
    while (wait(NULL) != -1){}
    return 0;
}

