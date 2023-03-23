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
    
    //сын в скобках
    if (fork() == 0){
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        
        //первый внук
        if (fork() == 0){
            execlp(argv[1], argv[1], NULL);
            exit(127);
        }
        
        int status;
        wait(&status);
        
        //если первый внук успешно выполнился
        if (WIFEXITED(status) && (WEXITSTATUS(status) == 0)){
            exit(0);
        }
        
        //иначе запускаем второго внука
        if (fork() == 0){
            execlp(argv[2], argv[2], NULL);
            exit(127);
        }
        
        wait(&status);
    
        if (WIFEXITED(status)){
            exit(WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)){
            exit(128 + WTERMSIG(status));
        }
    }
    
    int need_wait = fork();
    //второй сын
    if (need_wait == 0){
        dup2(fd[0], 0);
        close(fd[1]);
        close(fd[0]);
        execvp(argv[3], argv + 3);
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
    
