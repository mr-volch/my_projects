#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define READ_LEN 1000

void
print_out_str(int cur_lines, int cur_chars, char *name_one, char *name_two)
{
    write(1, name_one, strlen(name_one));
    write(1, " differs from ", strlen(" differs from "));
    write(1, name_two, strlen(name_two));
    write(1, ": line ", strlen(": line "));
        
    int line_exp = 0;
    int line_upper = 1;
        
    while (line_upper <= cur_lines){
        line_exp++;
        line_upper*=10;
    }
        
    char *lines_str = malloc(line_exp * sizeof(int));
        
    for (int i = 0; i < line_exp; i++){
        line_upper /= 10;
        lines_str[i] = (cur_lines / line_upper) + '0';
        cur_lines = cur_lines % line_upper;
    }
        
    write(1, lines_str, line_exp);
        
    free(lines_str);
        
    write(1, " char ", strlen(" char "));
        
    int char_exp = 0;
    int char_upper = 1;
        
    while (char_upper <= cur_chars){
        char_exp++;
        char_upper*=10;
    }
        
    char *chars_str = malloc(char_exp * sizeof(int));
        
    for (int i = 0; i < char_exp; i++){
        char_upper /= 10;
        chars_str[i] = (cur_chars / char_upper) + '0';
        cur_chars = cur_chars % char_upper;
    }
        
    write(1, chars_str, char_exp);
        
    free(chars_str);
    
    write(1, "\n", 1);
}

int
my_cmp(int fd_one, int fd_two, char *name_one, char *name_two)
{
    //буферы для чтения из файла
    char buf_one[READ_LEN + 1];
    char buf_two[READ_LEN + 1];
        
    //переменные для хранения
    // информации о колве
    // прочтенных симолов
    int n_one;
    int n_two;
    
    int cur_lines = 1;
    int cur_chars = 1;
    
    while (1){
       
        //читаем пока есть что читать
        n_one = read(fd_one, buf_one, READ_LEN);
        n_two = read(fd_two, buf_two, READ_LEN);
        
        if ((n_one < 0) || (n_two < 0)){
            write(2, "read failure\n", strlen("read failure\n"));
            return 1;
        }
        
        if ((n_one == 0) && (n_two == 0)){
            break;
        }
        
        if (((n_one == 0) && (n_two != 0)) || ((n_two == 0) && (n_one != 0))){
            print_out_str(cur_lines, cur_chars, name_one, name_two);
            break;
        }
        
        buf_one[n_one] = '\0';
        buf_two[n_two] = '\0';
        
        if (strcmp(buf_one, buf_two) == 0){
        
            char *cur = buf_one;
            char *prev = buf_one;
            
            while ((cur = strchr(cur, '\n')) != NULL){
                cur_lines++;
                cur = cur + 1;
                prev = cur;
                cur_chars = 1;
            }
            
            cur_chars += n_one - (prev - buf_one);
        } else{
            if (n_one > n_two){
                n_one = n_two;
            }
            
            for (int i = 0; i < n_one; i++){
                if (buf_one[i] == buf_two[i]){
                    if (buf_one[i] == '\n'){
                        cur_lines++;
                        cur_chars = 1;
                        continue;
                    }
                    cur_chars++;
                    continue;
                }
                break;
            }
            
            print_out_str(cur_lines, cur_chars, name_one, name_two);
            break;
        }
    }
    
    return 0;
}
            
        


int
main(int argc, char **argv)
{
    if (argc < 3){
        write(2, "wrong argument string\n", strlen("wrong argument string\n"));
        return 1;
    }
    
    int fd_one = open(argv[1], O_RDONLY);
    int fd_two = open(argv[2], O_RDONLY);
    
    if ((fd_one == -1) || (fd_two == -1)){
        write(2, "open failure\n", strlen("open failure\n"));
        close(fd_one);
        close(fd_two);
        return 1;
    }
    
    int res = my_cmp(fd_one, fd_two, argv[1], argv[2]);
    
    close(fd_one);
    close(fd_two);
    
    return res;
}
