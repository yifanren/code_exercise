#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define MAX_TEXT 512
#define MSGKEY 1281

struct msgbuf
{
    long int mtype;
    char     mtext[MAX_TEXT];
};

const char* msgHello = "hello";
const char* msgBye   = "bye";

void err(char * msgerr)
{
    printf("%s failed\n",msgerr);
    exit(1);
}

int main(void)
{
    pid_t pid;
    int   msgid;
    int   ret;
    struct msgbuf buf;
    int status;

    //creat message queue
    msgid = msgget(MSGKEY, IPC_CREAT | 0666);
    if (msgid == -1) {
        printf("msgget failed\n");
        exit(1);
    }

    if ((pid = fork()) < 0)
        err("fork");

    if (pid == 0) {
        if (msgrcv(msgid, &buf, MAX_TEXT, 1, 0) == -1)
            err("msgrcv");

        if (!strncmp(buf.mtext, msgHello, strlen(msgHello) + 1)) {
            printf("child success\n");

            strcpy(buf.mtext , msgBye);
            buf.mtype = 2;

            ret = msgsnd(msgid, &buf, sizeof(buf.mtext), 0);//send msg to queue
            if(ret == -1)
                err("child msgsnd");
            exit(1);
        }
    } else {
        strcpy(buf.mtext, msgHello);
        buf.mtype = 1;

        ret = msgsnd(msgid, &buf, sizeof(buf.mtext), 0);
        if (ret == -1)
            err("parent msgsnd");

        if (msgrcv(msgid, &buf, MAX_TEXT, 2, 0) == -1)
            err("parent msgrcv");

        wait(&status);

        if (!strncmp(buf.mtext, msgBye, strlen(msgBye) + 1)) {
            printf("parent success\n");
            exit(1);    
        }
    }
    return 0; 
}
