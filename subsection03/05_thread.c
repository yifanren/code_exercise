#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct message{
    int num;
    int64_t p;  // 用于第1/2/3线程，最后应变为3e+9
    int64_t q;  // 用于第1/2/4线程，最后应变为3e+9
    int64_t r;  // 用于第1/3线程，最后应变为2e+9
    int64_t s;  // 用于第1/2/3/4线程，最后应变为4e+9*
};

void thread(struct message *msg);

int main(void)
{
    pthread_t th;
    int ret;
    struct message *msg;
    int i;


    msg=(struct message*)malloc(sizeof(struct message));


    for(i=0;i<4;i++){
        msg->num=i;
        ret = pthread_create(&th, NULL, thread, msg);

        if(ret!=0){
            printf("create thread failed\n");
            exit(1);
        }
        pthread_join(th,NULL);
    }


    printf("p ==> %d,q==>%d,r==>%d,s==>%d\n",msg->p,msg->q,msg->r,msg->s);


    return 0;
}


void thread(struct message *msg)
{
    switch(msg->num){
        case 0:

            printf("----------- 0 ------->%s\n",msg->num);
            break;
        case 1:
            printf("----------- 1 ------->%d\n",msg->num);
            break;
        case 2:
            printf("----------- 2 ------->%d\n",msg->num);
            break;
        case 3:
            printf("----------- 3 ------->%d\n",msg->num);
            break;
    }
}

