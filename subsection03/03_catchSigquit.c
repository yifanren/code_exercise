#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

void SigHandler(int signum)
{
    if (signum == SIGALRM) {
        printf("hello\n");
        alarm(1);
    } 
    else if (signum == SIGQUIT || signum == SIGTERM) {
        printf("bye\n");
        exit(1);
    } 
}

int main(void)
{
    signal(SIGALRM,SigHandler);
    signal(SIGTERM,SigHandler);
    signal(SIGQUIT,SigHandler);

    alarm(1);
    while(1) {
        pause();
    }

    return 0;
}

