#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
    int fd0;
    if ((fd0 = open(argv[argc-1], O_APPEND|O_WRONLY|O_CREAT, 0666)) == -1){
        printf("%d\n", 1);
        return 1;
    }
    
    argv[argc-1] = NULL;
    if (fork() == 0){
        dup2(fd0, 1);
        close(fd0);
        execvp(argv[1], argv + 1);
        return 127;
    }
    
    close(fd0);
    int status_two;
    wait(&status_two);
    
    if (WIFEXITED(status_two) == 1){
        printf("%d\n", WEXITSTATUS(status_two));
        exit(WEXITSTATUS(status_two));
    } else if (WIFSIGNALED(status_two) == 1){
        printf("%d\n", 128 + WTERMSIG(status_two));
        exit(128 + WTERMSIG(status_two));
    }
    return 0;
}
