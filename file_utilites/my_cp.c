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
my_cp(int fd_one, int fd_two)
{
    char buf[READ_LEN];
    int n;
    
    while(1){
        
        n = read(fd_one, buf, READ_LEN);
        
        if (n == -1){
            write(2, "read failure\n", strlen("read failure\n"));
            return 1;
        }
        
        if (n == 0){
            break;
        }
        
        int m = write(fd_two, buf, n);
        
        if (m == -1){
            write(2, "write failure\n", strlen("write failure\n"));
            return 1;
        }
    }
    
    return 0;
}
                  
        


int
main(int argc, char **argv)
{
    if (argc != 3){
        write(2, "wrong argument string\n", strlen("wrong argument string\n"));
        return 1;
    }
    
    struct stat s1, s2;  
    stat(argv[1], &s1);
    if (stat(argv[2], &s2) != -1){
        if (s1.st_ino == s2.st_ino){
            write(2, "one file ia a pointer to another\n", strlen("one file ia a pointer to another\n"));
            return 1;
        }
    }
    
    
    int fd_one = open(argv[1], O_RDONLY);
    int fd_two = open(argv[2], O_WRONLY|O_TRUNC|O_CREAT, 0644);
    
    if ((fd_one == -1) || (fd_two == -1)){
        write(2, "open failure\n", strlen("open failure\n"));
        
        close(fd_one);
        close(fd_two);
        return 1;
    }
    
    int res = my_cp(fd_one, fd_two);
    
    close(fd_one);
    close(fd_two);
    
    return res;
}
