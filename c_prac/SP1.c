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
    int i;
    for (i = 1; i < argc; i++){
        if ((argv[i][0] == '-') && (argv[i][1] == '-') && (argv[i][2] == '\0')){
            argv[i] = NULL;
            break;
        }
    }
    
    
    int fd[2];
    pipe(fd);
    
    if (fork() == 0){
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        execvp(argv[1], argv + 1);
        exit(127);
    }
    
    int need_wait = fork();
    
    if (need_wait == 0){
        dup2(fd[0], 0);
        close(fd[1]);
        close(fd[0]);
        execvp(argv[i + 1], argv + (i+1));
        exit(127);
    }
    
    close(fd[1]);
    close(fd[0]);
    
    int status;
    waitpid(need_wait, &status, 0);
    
    if (WIFEXITED(status)){
        exit(WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)){
        exit(128 + WTERMSIG(status));
    }
    
    wait(NULL);
}
    
    
