#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define READ_LEN 1000

int
my_tail(int fd1, char *flag)
{
    
    long long str_num = 10;
    
    
    //если флаг начинается с +
    if ((flag != NULL) && (flag[0] == '+')){
        
        //сохраняем число из флага
        str_num = atoll(flag + 1) - 1;
        
        //буфер для чтения из файла
        char buf[READ_LEN + 1];
        
        //переменная для хранения
        // информации о колве
        // прочтенных симолов
        int n;
        
        while (1){
       
            //читаем пока есть что читать
            n = read(fd1, buf, READ_LEN);
            
            if (n < 0){
                write(2, "read failure\n", strlen("read failure\n"));
                return 1;
            }
            
            if (n == 0){
                return 0;
            }
            
            //чтобы можно было работать
            // как со строкой
            buf[n] = '\0';
            
            //если уже нашли достаточное число
            // строк, то просто печатаем
            if (str_num <= 0){
                write(1, buf, strlen(buf));
                continue;
            }
            
            
            //если же еще не нашли,
            // будем искать /n
            char *cur = buf;
            
            while ((cur = strchr(cur, '\n')) != NULL){
            
                //если осталось найти 1 \n,
                // то это значит, что мы его только
                //что нашли. Выводим оставшуюся
                // строку за ним и выходим
                if (str_num <= 1){
                    cur = cur + 1;
                    write(1, cur, strlen(cur));
                    str_num--;
                    break;
                
                } else{
                    str_num--;
                    cur = cur + 1;
                }
            }        
        }
    }
    
    //здесь окажемся только если
    // flag начинается с - или отсутсвует
    if ((flag != NULL) && (flag[0] == '-')){
        //если надо, сохраняем
        // поданное число в str_num
        //(по умолчанию str_num равно 10
        str_num = atoi(flag + 1);
    }
    
    long long cur_point = lseek(fd1, 0, SEEK_END);
    
    //если файл пустой
    if (cur_point == 0){
        return 0;
    }
    
    //если последний символ - перенос строки,
    //то ищем на один перенос больше
    
    lseek(fd1, -1, SEEK_END);
    char buf[READ_LEN + 1];
    
    int n = read(fd1, buf, 1);
    
    if (n < 0){
        write(2, "read failure\n", strlen("read failure\n"));
        return 1;
    }
    
    if (buf[0] == '\n'){
        str_num++;
    }
    
    long long lines_found = 0;
     
    while (1){
        
        int movement = READ_LEN;
        if (cur_point < movement){
            movement = cur_point;
        }
        
        lseek(fd1, -movement, SEEK_CUR);
        
        n = read(fd1, buf, movement);
        
        cur_point = lseek(fd1, -movement, SEEK_CUR);
        
        if (n < 0){
            write(2, "read failure\n", strlen("read failure\n"));
            return 1;
        }
        
        int i;
        
        for (i = (n-1); i >= 0; i--){
            if (buf[i] == '\n'){
                lines_found++;
            }
            
            if (lines_found == str_num){
                break;
            }
        }
        
        if (lines_found == str_num){
            lseek(fd1, i + 1, SEEK_CUR);
            break;
        }
        
        if (movement < READ_LEN){
            break;
        }
    }
    
    while (1){
        n = read(fd1, buf, READ_LEN);
        
        if (n < 0){
            write(2, "read failure\n", strlen("read failure\n"));
            return 1;
        }
        
        if (n == 0){
            break;
        }
        
        write(1, buf, n);
    }
    
    return 0;
}
    
    


int
main(int argc, char **argv)
{
    if (argc < 2){
        write(2, "wrong argument string\n", strlen("wrong argument string\n"));
        return 1;
    }
    
    int fd = 0;
    char *flag = NULL;
    
    if (argc == 2){   
        fd = open(argv[1], O_RDONLY);
    
        if (fd == -1){
            write(2, "open failure\n", strlen("open failure\n"));
            close(fd);
            return 1;
        }
    }
    
    if (argc > 2){
        flag = argv[1];
        
        fd = open(argv[2], O_RDONLY);
        if (fd == -1){
            write(2, "open failure\n", strlen("open failure\n"));
            close(fd);
            return 1;
        }
    } 
    
    int res = my_tail(fd, flag);
    
    close(fd);
    return res;
}
