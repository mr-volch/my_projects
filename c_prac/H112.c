#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

//для иллюстрации корректности работы 
//была выбрана следующая стратегия:
//в функции-обработчике SIGINT процесс
// самому себе посылает SIGTRAP
//после этого каждый четный приход
// сигнала созадется два дочерних процесса,
//связанных неименованным каналом. 
//через неименованный канал передаются два
//числа, которые выводятся вторым процессом
// на стандартный поток вывода
//при каждом приходе в функцию обработчик
// сигнала печатется сообщение о его приходе

volatile int count_int = 0;
volatile int count_trap = 0;
volatile int target_pid;

void
handle_sig_trap (int s)
{
    count_trap++;
    
    if (getpid() == target_pid){
        printf("\n I got SIGTRAP %d times\n", count_trap);
    }
    
    if ((count_trap % 2) == 0){
        
        int fd[2];
        pipe(fd);
        
        if (fork() == 0){
            //signal(SIGINT, SIG_DFL);
            int buf[3];
            buf[0] = 11;
            buf[1] = 22;
            write(fd[1], buf, 2*sizeof(int));
            close(fd[0]);
            close(fd[1]);
            exit(0);
        }
        
        if (fork() == 0){
            //signal(SIGINT, SIG_DFL);
            int buf_one[3];
            read(fd[0], buf_one, 2*sizeof(int));
            
            printf("%d   %d\n", buf_one[0], buf_one[1]);
            close(fd[0]);
            close(fd[1]);
            exit(0);
        }
        
        close(fd[0]);
        close(fd[1]);
        
    }
    
    signal(SIGTRAP, handle_sig_trap);
}
        

void
handle_sig_int (int s)
{
    count_int++;
    printf("\n I got SIGINT %d times\n", count_int);
    if (count_int == 6){
        while (wait(NULL) != (-1)){}
        signal(SIGINT, SIG_DFL);
    }
    
    else{
        signal(SIGINT, handle_sig_int);
    }
    
    kill(target_pid, SIGTRAP);
}

int 
main(void)
{
    target_pid = getpid();
    signal(SIGINT, handle_sig_int);
    signal(SIGTRAP, handle_sig_trap);
    while (1){}
    return 0;
}
