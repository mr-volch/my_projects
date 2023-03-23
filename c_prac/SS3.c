#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef void (*sighandler_t)(int);

volatile int main_flag = 0;
volatile int son_flag = 0;

void
son_my_sig_hndlr(int s)
{
    son_flag = 1;
    
}

void
father_my_sig_hndlr(int s)
{
    main_flag = 1;
}

void
term_sig_hndlr(int s)
{
    exit(0);
}

int 
main(void)
{
    int num;
    
    int fd[2];
    pipe(fd);
    
    signal(SIGUSR1, son_my_sig_hndlr);
    sighandler_t term_prev_ghdlr = signal(SIGTERM, term_sig_hndlr);
    
    pid_t new_pid = fork();
    
    if (new_pid == 0){
        while(1){
            if (son_flag == 1){
                son_flag = 0;
                int num_one;
                read(fd[0], &num_one, sizeof(int));
                //printf("SON %d\n", num_one);
                num_one--;
                write(fd[1], &num_one, sizeof(int));
                kill(getppid(), SIGUSR1);
            }
            usleep(100);
        }
    }
    
    signal(SIGUSR1, father_my_sig_hndlr);
    signal(SIGTERM, term_prev_ghdlr);
    
    while (scanf("%d", &num) != EOF){
        //printf("FATHER %d\n", num);
        write(fd[1], &num, sizeof(int));
        kill(new_pid, SIGUSR1);
        while (1){
            if (main_flag == 1){
                main_flag = 0;
                break;
            }
            usleep(100);
        }
        read(fd[0], &num, sizeof(int));
        printf("%d ", num);
        
    }
    
    kill(new_pid, SIGTERM);
    wait(NULL);
    return 0;
}
