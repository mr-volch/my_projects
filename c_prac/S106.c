#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
    char temp[7] = {'X', 'X', 'X', 'X', 'X', 'X', '\0'};
    int fd0 = mkstemp(temp);
    unlink(temp);
    
    int len = strlen(argv[1]);
    char main[len];
    
    write(fd0, argv[1], len);
    lseek(fd0, 0, SEEK_SET);
    read(fd0, main, len);
    
    write(1, main, len);
    printf("\n");
    
    close(fd0);
    
    return 0;
}
