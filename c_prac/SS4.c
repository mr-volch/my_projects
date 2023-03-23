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

void
int_sig_hndlr(int s)
{
    exit(1);
}


int
str_from_file_to_pipe(int file_fd, int pipe_fd, int str_num)
{
    char buf[READ_LEN + 1];
    
    int n;
    int local_flag = 0;
    
    while(1){
        n = read(file_fd, buf, READ_LEN);
        
        if (n < 0){
            return 1;
        }
        
        if (n == 0){
            break;
        }
        
        buf[n] = '\0';
        
        char *cur = buf;
        char **prev = malloc(sizeof(char**));
        *prev = buf;
        
        while ((cur = strchr(cur, '\n')) != NULL){
            
            if (str_num == 0){
                //write(1, "yah\n", strlen("yah\n"));
                if (local_flag == 0){
                    kill(getppid(), SIGUSR1);
                    local_flag = 1;
                }
                write(pipe_fd, *prev, (cur-*prev));
                free(prev);
                write(pipe_fd, "\n", 1);
                return 0;
            }
            
            //write(1, "yah\n", strlen("yah\n"));
            cur = cur + 1;
            *prev = cur;
            str_num--;
        }
        
        if (str_num == 0){
            if (local_flag == 0){
                kill(getppid(), SIGUSR1);
                local_flag = 1;
            }
            write(pipe_fd, *prev, (buf - *prev + n));
        }
        
        if (str_num < 0){
            free(prev);
            if (local_flag == 0){
                kill(getppid(), SIGUSR1);
                local_flag = 1;
            }
            write(pipe_fd, "\n", 1);
            return 0;
        }
        
        free(prev);
    }
    return 0;
}

int 
main(int argc, char **argv)
{
    int num;
    
    int fd[2];
    pipe(fd);
    //int file = open(argv[1], O_RDONLY);
    //str_from_file_to_pipe(file, fd[0], 13);
    
    signal(SIGINT, int_sig_hndlr);
    signal(SIGUSR1, son_my_sig_hndlr);
    sighandler_t term_prev_ghdlr = signal(SIGTERM, term_sig_hndlr);
    
    pid_t new_pid = fork();
    
    if (new_pid == 0){
        int file = open(argv[1], O_RDONLY);
        
        if (file == -1){
            kill(getppid(), SIGINT);
            exit(0);
        }
        
        while(1){
            if (son_flag == 1){
                son_flag = 0;
                int num_one;
                read(fd[0], &num_one, sizeof(int));
                lseek(file, 0, SEEK_SET);
                if (str_from_file_to_pipe(file, fd[1], num_one) == 1){
                    kill(getppid(), SIGINT);
                    exit(0);
                }
                //kill(getppid(), SIGUSR1);
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
        char cur;
        while (1){
            read(fd[0], &cur, 1);
            if (cur == '\n'){
                break;
            }
            printf("%c", cur);
        }
        printf("%c", cur);
    }
    
    kill(new_pid, SIGTERM);
    wait(NULL);
    return 0;
}
