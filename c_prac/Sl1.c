#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

struct Message{
    long mtype;
    char Data[258];
};


union semun{
    int val;
    struct semid *buf;
    ushort *array;
};


int
make_son(int argc, char **argv)
{
    signal(SIGINT, SIG_DFL);
    
    //получаем ключ
    key_t key = ftok(argv[0], 's');
    
    //создаем массив из 2 семафоров
    int semid = semget(key, 2, 0);
    
    if (semid == -1){
        exit(1);
    }
    
    //создаем разделяемую память оазмера 1024 байта(килобайт)
    int shmid = shmget(key, 1024, 0);
    
    if (shmid == -1){
        exit(1);
    }
    
    //создаем очередь сообщений
    int msgid = msgget(key, 0);
        
    if (msgid == -1){
        exit(1);
    }
    
    exit(1);
    
    return 0;
}


static int semid = -1;
static int shmid = -1;
static int msgid = -1;


void
int_handler(int s)
{
    if (semid != -1){
        semctl(semid, 0, IPC_RMID);
    }
    
    if (shmid != -1){
        shmctl(shmid, IPC_RMID, 0);
    }
    
    if (msgid != -1){
        msgctl(msgid, IPC_RMID, 0);
    }
    
    exit(0);
}


int
main(int argc, char **argv)
{
    key_t key;
    signal(SIGINT, int_handler);
    
    //получаем ключ
    if ((key = ftok(argv[0], 's')) == -1){
        int_handler(0);
    }
    
    //создаме массив семафоров
    if ((semid = semget(key, 2, 0666 | IPC_CREAT)) == -1){
        int_handler(0);
    }
    
    //создаем разделяемую память размером килобайт
    if ((shmid = shmget(key, 1024, 0666 | IPC_CREAT)) == -1){
        int_handler(0);
    }
    
    //создаем очередт сообщений    
    if ((msgid = msgget(key, 0666 | IPC_CREAT)) == -1){
        int_handler(0);
    }
        
    union semun union_1 = {.val = 0};
    
    //инициализация массива семафоров
    semctl(semid, 0, SETVAL, union_1);
    semctl(semid, 1, SETVAL, union_1); 
    
    if (fork() == 0){
        return (make_son(argc, argv));
    }
    
    
    while (wait(NULL) != -1){}
    return 0;   
}
