#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int
main(int argc, char **argv)
{

    if (argc == 1){
        exit(0);
    }
    
    if (argc == 2){
        
        if (fork() == 0){
            execlp(argv[1], argv[1], NULL);
            exit(127);
        }
        
        int status;
        wait(&status);
    
        if (WIFEXITED(status)){
            exit(WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)){
            exit(128 + WTERMSIG(status));
        }
    }
        
        

    int last_pr = 0;

    int fd_one[2];
    pipe(fd_one);
    
    
    if ((last_pr = fork()) == 0){
        dup2(fd_one[1],1);
        close(fd_one[1]);
        close(fd_one[0]);
        execlp(argv[1], argv[1], NULL);
        exit(127);
    }
        
    int fd_two[2];
    
    for (int i = 2; i < (argc-1); i++){
        fd_two[0] = fd_one[0];
        fd_two[1] = fd_one[1];
        pipe(fd_one);
        
        close(fd_two[1]);
        
        if ((last_pr = fork()) == 0){
            dup2(fd_two[0], 0);
            dup2(fd_one[1], 1);
            
            close(fd_one[0]);
            close(fd_one[1]);
            close(fd_two[0]);
            execlp(argv[i], argv[i], NULL);
            exit(127);
        }
        
        close(fd_two[0]);
    }
    
    close(fd_one[1]);
    
    if((last_pr = fork()) == 0){
        dup2(fd_one[0], 0);
        close(fd_one[0]);
        execlp(argv[argc-1], argv[argc-1], NULL);
        exit(127);
    }
    
    close(fd_one[0]);
    
    int status;
    waitpid(last_pr, &status, 0);
    
    int to_exit = 0;
    
    if (WIFEXITED(status)){
        to_exit = (WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)){
        to_exit = (128 + WTERMSIG(status));
    }
    
    while (wait(NULL) != -1){}
    
    exit(to_exit);
}
    
