#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <atomic>

using namespace std;

#define MAX_THREAD_NUM  (4)
#define MAX_COUNT       (1E+7)

atomic<long> pp {0};
atomic<long> qq {0};
atomic<long> rr {0};
atomic<long> ss {0};

int64_t p = 0;  
int64_t q = 0;
int64_t r = 0;
int64_t s = 0;

pthread_mutex_t pLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t qLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sLock = PTHREAD_MUTEX_INITIALIZER;


void* TestMutex(void* id)
{
    int num = *(int *)id;
    int i = 0;   

    while(i < MAX_COUNT) {
        if (num & (1 << 0)) {    
            pthread_mutex_lock(&pLock);
            p++;
            pthread_mutex_unlock(&pLock);
        }

        if (num & (1 << 1)) {    
            pthread_mutex_lock(&qLock);
            q++;
            pthread_mutex_unlock(&qLock);
        }


        if (num & (1 << 2)) {    
            pthread_mutex_lock(&rLock);
            r++;
            pthread_mutex_unlock(&rLock);
        }

        if (num & (1 << 3)) {
            pthread_mutex_lock(&sLock);
            s++;
            pthread_mutex_unlock(&sLock);
        }

        i++;
    }

    return NULL;
}

void* TestAtomic(void* id)
{
    int num = *(int *)id;
    int i = 0;   

    while(i < MAX_COUNT){
        if(num & (1 << 0)){    
            pp++; 
        }

         if(num & (1 << 1)){    
            qq++;
         }

         if(num & (1 << 2)){    
           rr++;
         }

         if(num & (1 << 3)){
           ss++;
         }
         i++;
    }

    return NULL;
}

int main(void)
{
    int ret, i;
    pthread_t th[MAX_THREAD_NUM];
    int id[MAX_THREAD_NUM];
    time_t startTime, endTime;   

    id[0] = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
    id[1] = (1 << 0) | (1 << 1) | (1 << 3);
    id[2] = (1 << 0) | (1 << 2) | (1 << 3);
    id[3] = (1 << 1) | (1 << 3);

    time(&startTime);
    for (i = 0; i < MAX_THREAD_NUM; i++) {
        ret = pthread_create(&th[i], NULL, TestMutex, &id[i]);

        if(ret != 0){
            printf("create thread failed\n");
            exit(1);
        }
    }

    for (i = 0; i < MAX_THREAD_NUM; i++) {
        pthread_join(th[i], NULL);
    }
    
    time(&endTime);
    
    printf("p : %ld,\tq : %ld,\ts : %ld,\tr : %ld\n", p, q, s, r);
    printf("mutex running time : %ld s\n", endTime - startTime);
   
    //atomic
    time(&startTime);

    std::thread t1 = thread(TestAtomic, &id[0]);
    std::thread t2 = thread(TestAtomic, &id[1]);
    std::thread t3 = thread(TestAtomic, &id[2]);
    std::thread t4 = thread(TestAtomic, &id[3]);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    time(&endTime);
    
    cout << "pp : " << pp << "\tqq :" << qq << "\tss : "<< ss << "\trr : " <<rr <<endl; 
    printf("atomic running time : %ld s\n", endTime - startTime);

    return 0;
}
