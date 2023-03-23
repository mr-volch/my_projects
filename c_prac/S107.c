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
copy_file(int fd1, int fd2)
{   
    char buf[READ_LEN + 1];
    int n;
    int s = 0;
    while (1){
       
        n = read(fd1, buf, READ_LEN);
        if (n == 0){
            return -1;
        }
        buf[n] = '\0';
        char *cur;
        if ((cur = strchr(buf, '\n')) != NULL){
            int cur_wr = write(fd2, buf, cur - buf + 1);
            if (cur_wr != cur - buf + 1){
                return -1;
            }
            s += (cur - buf + 1);
            return s;
        }
        int cur_wr = write(fd2, buf, READ_LEN);
        if (cur_wr != READ_LEN){
            return -1;
        }
        s += READ_LEN;
    }
    
    return -1;
}
    

int
main(int argc, char **argv)
{
    struct stat s1, s2;  
    stat(argv[1], &s1);
    if (stat(argv[2], &s2) == -1){
        int fd1 = open(argv[1], O_RDONLY);
        int fd2 = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0666);
        copy_file(fd1, fd2);
         
        close(fd1);
        close(fd2);
        
        return 0;
    }
    
    if (s1.st_ino == s2.st_ino){
        int fd1 = open(argv[1], O_RDONLY);
        char temp[7] = {'X', 'X', 'X', 'X', 'X', 'X', '\0'};
        int fd_tmp = mkstemp(temp);
        unlink(temp);
        copy_file(fd1, fd_tmp);
        lseek(fd_tmp, 0, SEEK_SET);
        close(fd1);
        int fd2 = open(argv[1], O_WRONLY|O_TRUNC);
        copy_file(fd_tmp, fd2);
        close(fd_tmp);
        close(fd1);
        return 0;        
    }

    int fd1 = open(argv[1], O_RDONLY, 0);
    int fd2 = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0666);
    
    copy_file(fd1, fd2);
     
    close(fd1);
    close(fd2);
    
    return 0;
}
