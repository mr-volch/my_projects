#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include<sys/wait.h>

//предполагается, что программа будет собрана в файл a.out

struct {
    long mtype;
    char Data[258];
} Message;


int
main(int argc, char **argv)
{
    key_t key;
    int msgid;
    char str_fact[258];
    
    
    key = ftok("a.out", 's');
    
    msgid = msgget(key, 0666 | IPC_CREAT);
    
    if (fork() == 0){
        for(;;){
            msgrcv(msgid, (struct msgbuf*) (&Message), 258, 1, 0);
            if ((Message.Data[0] == 'q') || (Message.Data[0] == 'Q')){
                break;
            }
            write(1, "A: ", 3);
            write(1, Message.Data, strlen(Message.Data));
        }
        exit(0);
    }
    
    if (fork() == 0){
        for(;;){
            msgrcv(msgid, (struct msgbuf*) (&Message), 258, 2, 0);
            if ((Message.Data[0] == 'q') || (Message.Data[0] == 'Q')){
                break;
            }
            write(1, "B: ", 3);
            write(1, Message.Data, strlen(Message.Data));
        }
        exit(0);
    }
    
    
    for(;;){
        fgets(str_fact, 258, stdin);
        //str_fact[strlen(str_fact) - 1] = '\0';
        strcpy(Message.Data, str_fact);
        
        if ((str_fact[0] == 'a') || (str_fact[0] == 'A')){
            Message.mtype = 1;
            msgsnd(msgid,(struct msgbuf*) (&Message), strlen(str_fact) + 1, 0);
        }
        
        if ((str_fact[0] == 'b') || (str_fact[0] == 'B')){
            Message.mtype = 2;
            msgsnd(msgid,(struct msgbuf*) (&Message), strlen(str_fact) + 1, 0);
        }
        
        if ((str_fact[0] == 'q') || (str_fact[0] == 'Q')){
            Message.mtype = 1;
            msgsnd(msgid,(struct msgbuf*) (&Message), strlen(str_fact) + 1, 0);
            Message.mtype = 2;
            msgsnd(msgid,(struct msgbuf*) (&Message), strlen(str_fact) + 1, 0);
            break;
        }
    }
    
    while ((wait(NULL)) != -1){}
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
        
            
        
