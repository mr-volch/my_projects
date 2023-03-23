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
    //int fd2 = open("test_4343", O_WRONLY|O_TRUNC);
    
    int str_num = 10;
    
    
    //если флаг начинается с +
    if ((flag != NULL) && (flag[0] == '+')){
        
        //сохраняем число из флага
        str_num = atoi(flag + 1) - 1;
        
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
                //write(fd2, buf, strlen(buf));
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
                    //write(fd2, cur, strlen(cur));
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
    
    //будем хранить смещения
    // от начала файла
    // по которым хрянятся
    // последние прочтенные n строк
    int *last_seeks = malloc(str_num * sizeof(int));
    int seeks_len = str_num;
    
    //первая строка всегда
    // имеет нулевое смещение
    last_seeks[0] = 0;
    
    //буфер для чтения из файла
    char buf[READ_LEN + 1];
    
    //кол-во прочтенных 
    //символов за одно чтение
    int n;
    
    //общее кол-во прочтенных
    // символов за все время
    int s = 0;
    
    while (1){
    
        //читаем пока есть что читать
        n = read(fd1, buf, READ_LEN);
        
        if (n < 0){
            write(2, "read failure\n", strlen("read failure\n"));
            return 1;
        }
        
        if (n == 0){
            break;
        }
        
        //чтобы можно было
        // работать как со строкой
        buf[n] = '\0';
        
        //ищем переносы строки
        char *cur = buf;
            
        while ((cur = strchr(cur, '\n')) != NULL){
            //если уже нашли достаточное
            // количество строк
            if (str_num == 1){
            
                //сдвигаем на один влево и в
                // конец добавляем новый элемент;
                for (int j = 0; j < seeks_len-1; j++){
                    last_seeks[j] = last_seeks[j+1];
                }
                
                last_seeks[seeks_len - 1] = (s + (cur - buf) + 1);
                cur = cur + 1;
            
            //иначе же просто записываем на
            // следующее свободное место
            //новый элемент: s - колво
            // прочтений блоков до текущего,
            //((cur - buf) + 1) - внутреннее
            // смещение в блоке
            } else{
                str_num--;
                last_seeks[seeks_len - str_num] = (s + (cur - buf) + 1);
                cur = cur + 1;
            }
        }
        
        //добавялем к счетчику прочтенных
        // байтов длину одного блока
        s += READ_LEN;        
    }
    
    //двигаем указатель файла на начало 
    //n-ой строки с конца
    lseek(fd1, last_seeks[0], SEEK_SET);
    
    //чистим память массива смещений
    free(last_seeks);
    
    //печатаем все до конца
    // с установленной точки
    while (1){
        n = read(fd1, buf, READ_LEN);
        
        if (n < 0){
            write(2, "read failure\n", strlen("read failure\n"));
            return 1;
        }
        
        if (n == 0){
            return 0;
        }
        buf[n] = '\0';
        write(1, buf, strlen(buf));
        //write(fd2, buf, strlen(buf));
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
            return 1;
        }
    }
    
    if (argc > 2){
        flag = argv[1];
        
        fd = open(argv[2], O_RDONLY);
        if (fd == -1){
            write(2, "open failure\n", strlen("open failure\n"));
            return 1;
        }
    } 
    
    int res = my_tail(fd, flag);
    close(fd);
    return res;
}
    
