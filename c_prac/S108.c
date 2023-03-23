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
single_copy(int fd1)
{
    char temp[7] = {'X', 'X', 'X', 'X', 'X', 'X', '\0'};
    int fd_tmp = mkstemp(temp);
    unlink(temp);
    if (copy_file(fd1, fd_tmp) == -1){
        return -1;
    }
    return fd_tmp;
}

int
reverse_copy(int fd1)
{
    char buf[READ_LEN + 1];
    int *fd_temps = NULL;
    int n, prev_n, temps_len = 1;
    int s = 0;
    while (1){
       
        prev_n = n;
        n = read(fd1, buf, READ_LEN);
        if (n == 0){
            break;
        }
        buf[n] = '\0';
        for(int i = 0; i < n/2; i++){
            char cur = buf[i];
            buf[i] = buf[n-1-i];
            buf[n-1-i] = cur;
        }
        
        char temp[7] = {'X', 'X', 'X', 'X', 'X', 'X', '\0'};
        int fd_tmp = mkstemp(temp);
        unlink(temp);
        write(fd_tmp, buf, n);
        
        temps_len++;
        fd_temps = realloc(fd_temps, temps_len * (sizeof(int)));
        fd_temps[temps_len-1] = fd_tmp;
    }
    if fd_temps = NULL{
        return 0;
    }
    
    lseek(fd1, -(READ_lEN * (temps_len-1) + prev_n), SEEK_CUR);
    
    lseek(fd_temps[temps_len - 1], 1, SEEK_SET);
    n = read(fd_temps[temps_len-1], buf, READ_LEN);
    write(fd1, buf, n);
    
    for (int j = temps_len - 2; j >= 0; j--){
        lseek(fd_temps[j], 0, SEEK_SET);
        read(fd_temps[j], buf, READ_LEN);
        write(fd1, buf, READ_LEN);
    }
    
    return 0;
    
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
