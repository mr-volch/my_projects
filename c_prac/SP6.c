#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


#define READ_LEN 1000

//разделяемым ресурсом процессов является
// неименованный канал
//утилиты grep в сыновьих процессах записывают
// в него найденные строки
//в отцовском процессе происходит чтение
// из неименованного канала при 
//помощи read. Соответвенно, если отцовский процесс
// окажется в ситуации когда
//ему не хватает прочтенных данных,
// то он заблокируется на read, до того
//момента пока один из сыновьих процессов
// не запишет туда что либо

int
file_max_str(int fd)
{
    int till_end = 0;
    int max_len = 0;
    
    while (1){
    
        //читаем пока есть что читать
        char buf[READ_LEN + 1];
        
        //критическая секция: здесь процесс читает
        // из канала и остановится, ожидая что
        //сын запишет что-то в неименованный канал
        //(если пока что нечего читать)
        int n = read(fd, buf, READ_LEN);
        
        if (n <= 0){
            break;
        }
        
        //чтобы можно было работать как со строкой
        buf[n] = '\0';
        
        //ищем переносы строки
        char *cur = buf;
        char *prev = buf;
            
        while ((cur = strchr(cur, '\n')) != NULL){
            if (((cur - prev) + till_end) >= max_len){
                max_len = cur-prev+till_end;
            }
            cur = cur + 1;
            prev = cur;
            till_end = 0;
        }
        
        till_end = (n - (prev - buf)); 
    }  
    
    return max_len;
}

int
main(int argc, char **argv)
{
    int fd[2];
    pipe(fd);
    
    int fd_old[2];
    
    for (int i = 2; i < argc; i++){
        
        fd_old = fd;
        pipe(fd);
        
        if (fork() == 0){
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            
            execlp("grep", "grep", argv[1], argv[i], NULL);          
            exit(127);
        }
    }
    
    
    close(fd[1]);
    
    printf("%d\n", file_max_str(fd[0]));
    close(fd[0]);    
    
    while (wait(NULL) != -1){}
    return 0;
}



int
main(int argc, char **argv)
{

    if (argc == 2){
        exit(0);
    }
    
    else if (argc == 3){
    
        int fd[2];
        pipe(fd);
        
        if (fork() == 0){
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            execlp("grep", "grep", argv[1], argv[2], NULL);
            exit(127);
        }
        
        close(fd[1]);
    }
        
        

    else{ 
        int last_pr = 0;
        int pipes[argc-2];

        int fd_one[2];
        pipe(fd_one);
        pipes[0] = fd_one[0];
    
        if ((last_pr = fork()) == 0){
            dup2(fd_one[1],1);
            close(fd_one[1]);
            close(fd_one[0]);
            execlp("grep", argv[1], argv[2], NULL);
            exit(127);
        }
        
        int fd_two[2];
    
        for (int i = 3; i < argc; i++){
            fd_two[0] = fd_one[0];
            fd_two[1] = fd_one[1];
            pipe(fd_one);
            pipes[i-2] = fd_one[0];
        
            close(fd_two[1]);
        
            if ((last_pr = fork()) == 0){
                dup2(fd_two[0], 0);
                dup2(fd_one[1], 1);
            
                close(fd_one[0]);
                close(fd_one[1]);
                close(fd_two[0]);
                execlp("grep", "grep", argv[1], argv[i], NULL);
                exit(127);
            }
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
