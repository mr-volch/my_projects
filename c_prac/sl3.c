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

struct message{
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
    
    //сообщения двух типов - один для
    //первого сына, другой для второго
    struct message msg_one = {.mtype = 1, .Data = "a"};
    struct message msg_two = {.mtype = 2, .Data = "b"};
    //struct messgae msg_fin = {.mtype = 3, .Data = {'q', '\0'}};
    
    int N = atoi(argv[1]);
    int to_print = 0;
    if (argv[2][0] == 'b'){
        to_print = 1;
    }
        
    struct message msg_get_one = {.mtype = 1};
    struct message msg_get_two = {.mtype = 2};
    
    for(;;){
        //отсылаем сообщения соответсвующего типа
        if (to_print >= N){
            if (argv[2][0] == 'a'){
                msgsnd(msgid, &msg_two, 258, 0);
            }
        
            else{
                msgsnd(msgid, &msg_one, 258, 0);
            }
            break;
        }
        
        
        //считываем соотв. сообщение - 
        //вход в критическую секцию
        if (argv[2][0] == 'a'){
            //fprintf(stderr, "stop\n");
            msgrcv(msgid, &msg_get_one, 258, 1, 0);
        }
        
        else{
            msgrcv(msgid, &msg_get_two, 258, 2, 0);
        }
        
        printf("%d ", to_print);
        fflush(stdout);
        
        to_print += 2;
        
        //отсылаем соотв. сообщение -
        //выход из критической секции
        if (argv[2][0] == 'a'){
            //fprintf(stderr, "%ld \n", msg_two.mtype);
            //fflush(stdout);
            msgsnd(msgid, &msg_two, 258, 0);
        }
        
        else{
            msgsnd(msgid, &msg_one, 258, 0);
        }
    }
    
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
    
    struct message msg_one = {.mtype = 1};
    msgsnd(msgid, (struct message*) &msg_one, 258, 0);
    
    char *argv_son[4];
   
    argv_son[0] = argv[0];
    argv_son[1] = argv[1];
    char a[1] = {'a'};
    argv_son[2] = a;
    argv_son[3] = NULL;
    
    if (fork() == 0){
        return (make_son(3, argv_son));
    }
    
    
    argv_son[2][0] = 'b';
    
    if (fork() == 0){
        return (make_son(3, argv_son));
    }
    
    while (wait(NULL) != -1){}
    int_handler(0);
    return 0;   
}
