#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <atomic>
#define MAX_THREAD_NUM  (4)
#define MAX_COUNT       (1E+5)

using namespace std;

std::atomic_long p(0);
std::atomic_long q {0};
std::atomic_long r {0};
std::atomic_long s {0};

void *GetThread(void* id);

int main(void)
{
    int ret;
    int id[MAX_THREAD_NUM];
    int i;
    clock_t begintime, endtime;   

    id[0] = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
    id[1] = (1 << 0) | (1 << 1) | (1 << 3);
    id[2] = (1 << 0) | (1 << 2) | (1 << 3);
    id[3] = (1 << 1) | (1 << 3);

    begintime = clock();

    std::thread t1(GetThread, &id[0]);
    std::thread t2(GetThread, &id[1]);
    std::thread t3(GetThread, &id[2]);
    std::thread t4(GetThread, &id[3]);
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    endtime = clock();

    printf("p-->%ld, q-->%ld, r-->%ld, s-->%ld\n",p, q, r, s);
    printf("running time : %ld ms \n", endtime - begintime);

    return 0;
}

void* GetThread(void* id)
{
    int num = *(int *)id;
    int i = 0;   

    while(i < MAX_COUNT){
        if(num & (1 << 0)){    
            p++;
         }

         if(num & (1 << 1)){    
            q++;
         }


         if(num & (1 << 2)){    
            r++;
         }

         if(num & (1 << 3)){
            s++;
         }
         i++;
    }
}
