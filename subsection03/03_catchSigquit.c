#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>


/*
 *写一个程序，每隔一秒打印一行Hello，当捕捉到SIGQUIT或SIGTERM时终止程序，
 并在退出程序前打印Bye。测试方法为通过命令行向该程序的进程发送以上信号。
 * */

#if 0


void my_func(int signum)
{
    printf("Bye\n");

}

int main(void)
{
    sigset_t set;
    struct sigaction action1,action2;

    if(sigemptyset(&set)<0)
        perror("sigemptyset");

    if(sigaddset(&set,SIGQUIT)<0)
        perror("sigaddset");
    if(sigaddset(&set,SIGTERM)<0)
        perror("sigaddset sigterm failed");

    if(sigprocmask(SIG_BLOCK,&set,NULL)<0)
        perror("sigprocmask");
    else
    {
        printf("hello\n");
        sleep(1);
    }


    while(1){
        if(sigismember(&set,SIGQUIT)){
            sigemptyset(&action1.sa_mask);
            action1.sa_handler=my_func;
            sigaction(SIGQUIT,&action1,NULL);
        
        }else if(sigismember(&set,SIGTERM)){
            sigemptyset(&action2.sa_mask);
            action2.sa_handler=my_func;
            sigaction(SIGTERM,&action2,NULL);
        }

    }

    return 0;

}

#endif

void sig_usr(int signum)
{
    printf("bye\n");
    exit(1);

}

void psleep()
{
   printf("hello\n");
}

int main(void)
{

    signal(SIGALRM,psleep);
    signal(SIGTERM,sig_usr);
    signal(SIGQUIT,sig_usr);
    

    for(;;){
    alarm(1);
    pause();
   
}

    return 0;
}














