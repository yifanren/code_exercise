#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>


void sig_usr(int signum)
{
    printf("bye\n");
    exit(1);
}

void psleep()
{
   printf("hello\n");
   alarm(1);
}

int main(void)
{
    signal(SIGALRM,psleep);
    signal(SIGTERM,sig_usr);
    signal(SIGQUIT,sig_usr);

    alarm(1);
    
    while(1) {
        pause();
    }
   
    return 0;
}

