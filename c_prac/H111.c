
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
    //создаем неименованый канал
    int fd[2];
    pipe(fd);
    
    //в сыне отождествляем вывод с файловым дескриптором канала
    //и меняем тела сыновьего процесса на cat с соотв. параметрами
    if (fork() == 0){
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        execlp("cat", "cat", "/etc/passwd", NULL);
    }
    
    //в отце отождествялем ввод с файловыс дескриптором канала
    //и меняем тело отцовского процесса на grep с соотв. параметрами
    dup2(fd[0], 0 );
    close(fd[0]);
    close(fd[1]);
    execlp("grep", "grep", "user", NULL);
}
