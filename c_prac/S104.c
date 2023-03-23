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
    if (fork() == 0){
        dup2(1, 2);
        execvp(argv[1], argv + 1);
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
