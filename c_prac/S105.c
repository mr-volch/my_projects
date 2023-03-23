#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>



/* дерево процессов
       {процесс исходной программы}
         |                     |
         |                     |(запускается если процесс 
         |                     |в скобках успешно завершился)
         |                     |
  {процесс в скобках}       {процесс, запускающиий р3} 
    |              |
    |              |(запускается, если процесс 
    |              |запускающий p1 неуспешно завершился)
    |              |
 {процесс,        {процесс,
 запускающий р1}  запускающий р2}
 
*/

int main(int argc, char **argv)
{
    pid_t pid;
    pid = fork();
    
    if (pid == 0){
        int fd0;
        if ((fd0 = open(argv[3], O_WRONLY|O_CREAT|O_TRUNC, 0666)) == -1){
            return 1;
        }
        dup2(fd0, 1);
        close(fd0);
        pid_t pid_one;
        pid_one = fork();
        
        if (pid_one == 0){
            execlp(argv[1], argv[1], NULL);
            exit(127);
        }
        
        int status;
        wait(&status);
        
        if ((WIFEXITED(status)) && (WEXITSTATUS(status) == 0)){
            exit(0);
        }
        
        pid_t pid_two;
        pid_two = fork();
        
        if (pid_two == 0){
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
    
    int gl_status;
    wait(&gl_status);
    if ((WIFEXITED(gl_status) == 1) && (WEXITSTATUS(gl_status) == 0)){
        pid_t pid_three;
        pid_three = fork();
        
        
        if (pid_three == 0){
            execlp(argv[4], argv[4], NULL);
            exit(127);
        }

        wait(&gl_status);
    }
    
    if (WIFEXITED(gl_status)){
        exit(WEXITSTATUS(gl_status));
    } else if (WIFSIGNALED(gl_status)){
        exit(128 + WTERMSIG(gl_status));
    }
    return 0;
}
