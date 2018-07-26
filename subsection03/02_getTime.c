#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/time.h>


int main(void)
{
    time_t now;
    struct tm* tmNow;
    char formatedTime[80];
    struct timeval tv;
    struct timezone tz;

    //get time
    time(&now);

    //get localtime
    tmNow=localtime(&now);


    //format localtime
    strftime(formatedTime,sizeof(formatedTime),"%F %H:%M:%S",tmNow);

    //get system time
    gettimeofday(&tv,&tz);

    

    printf("unix system time is : %d-%d-%d %d:%d:%d\n",tmNow->tm_year,tmNow->tm_mon,
            tmNow->tm_mday,tmNow->tm_hour,tmNow->tm_min,tmNow->tm_sec);
    printf("localtime is : %s.%ld\n",formatedTime,tv.tv_usec);






    return 0;
}

