#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/time.h>


int main(void)
{
    struct tm* tmNow;
    struct timeval tv;
    char formatedTime[80];

    gettimeofday(&tv, NULL);
    tmNow = localtime(&tv.tv_sec);
    
    strftime(formatedTime, sizeof(formatedTime), "%F %H:%M:%S", tmNow);

    printf("unix time %ld sec\n", tv.tv_sec);
    printf("localtime is : %s.%03ld\n", formatedTime, tv.tv_usec/1000);

    return 0;
}

