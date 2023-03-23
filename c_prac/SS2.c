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

volatile int count_int = 0;

void
alrm_sig_hndlr(int s)
{
    count_int = 0;
}

void
my_sig_hndlr(int s)
{
    alarm(1);
    count_int++;
    printf("%d\n", count_int);
}

void
term_sig_hndlr(int s)
{
    printf("END\n");
    exit(0);
}


int 
main(void)
{
    int num;
    
    sighandler_t my_prev_ghdlr = signal(SIGINT, my_sig_hndlr);
    sighandler_t term_prev_ghdlr = signal(SIGTERM, term_sig_hndlr);
    sighandler_t alrm_prev_ghdlr = signal(SIGALRM, alrm_sig_hndlr);
    
    pid_t new_pid = fork();
    
    if (new_pid == 0){
        for(;;){
            usleep(100);
        }
    }
    
    signal(SIGINT, my_prev_ghdlr);
    signal(SIGTERM, term_prev_ghdlr);
    signal(SIGALRM, alrm_prev_ghdlr);
    
    while (scanf("%d", &num) != EOF){
        if (num == 0){
            kill(new_pid, SIGINT);
        } else{
            usleep(num);
        }
    }
    
    kill(new_pid, SIGTERM);
    wait(NULL);
    return 0;
}
