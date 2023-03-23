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
    if ((fd0 = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0666)) == -1){
        return 1;
    }

    dup2(fd0, 1);
    close(fd0);
    if (fork() == 0){
        execlp(argv[1], argv[1], NULL);
        return 127;
    }
    
    int status_two;
    wait(&status_two);
    
    if (WIFEXITED(status_two) == 1){
        exit(WEXITSTATUS(status_two));
    } else if (WIFSIGNALED(status_two) == 1){
        exit(128 + WTERMSIG(status_two));
    }
    return 0;
}
