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
    
    int N = atoi(argv[1]);
    int to_print = 0;
    
    //операция - увеличение значения семафора
    struct sembuf opmas_down[1];
    //операция - уменьшение значения семафора
    struct sembuf opmas_up[1];
    
    if (argv[2][0] == 'b'){
        to_print = 1;
        //во втором процессе, опускаем первый семафор,
        // поднимаем нулевой
        opmas_down[0].sem_num = 1;
        opmas_down[0].sem_op = -1;
        opmas_down[0].sem_flg = 0;
        
        opmas_up[0].sem_num = 0;
        opmas_up[0].sem_op = 1;
        opmas_up[0].sem_flg = 0;
    }
    
    if (argv[2][0] == 'a'){
        //в первом процессе, опускаемнулевой семафор,
        //поднимаем первый
        opmas_down[0].sem_num = 0;
        opmas_down[0].sem_op = -1;
        opmas_down[0].sem_flg = 0;
        
        opmas_up[0].sem_num = 1;
        opmas_up[0].sem_op = 1;
        opmas_up[0].sem_flg = 0;
    }
    
    for(;;){
        if (to_print >= N){
            //поднимаем соответсвующие семафоры.
            if (argv[2][0] == 'a'){
                semop(semid, opmas_up, 1);
            }
        
            else{
                semop(semid, opmas_up, 1);
            }
            break;
        }
        
        //опускаем соответсвующие семафоры -
        // вход в критическую секцию
        if (argv[2][0] == 'a'){
            //fprintf(stderr, "stop\n");
            semop(semid, opmas_down, 1);
        }
        
        else{
            semop(semid, opmas_down, 1);
        }
        
        printf("%d ", to_print);
        fflush(stdout);
        
        to_print += 2;
        
        //поднимаем соответсвующие семафоры
        //выход из критической секции
        if (argv[2][0] == 'a'){
            //fprintf(stderr, "%ld \n", msg_two.mtype);
            //fflush(stdout);
            semop(semid, opmas_up, 1);
        }
        
        else{
            semop(semid, opmas_up, 1);
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
        
    union semun union_1 = {.val = 1};
    union semun union_2 = {.val = 0};
    
    //инициализация массива семафоров
    semctl(semid, 0, SETVAL, union_1);
    semctl(semid, 1, SETVAL, union_2);
    
    
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
