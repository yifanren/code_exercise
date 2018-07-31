#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREAD_NUM  (4)
#define MAX_COUNT       (1E+5)

int64_t p = 0;  
int64_t q = 0;
int64_t r = 0;
int64_t s = 0;
clock_t runtime = 0;

pthread_mutex_t lock[MAX_THREAD_NUM];

void *thread(void* id);

int main(void)
{
  
    pthread_t th[MAX_THREAD_NUM];
    int ret;
    int id[MAX_THREAD_NUM];
    int i;
    clock_t begintime, endtime;   


    id[0] = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
    id[1] = (1 << 0) | (1 << 1) | (1 << 3);
    id[2] = (1 << 0) | (1 << 2) | (1 << 3);
    id[3] = (1 << 1) | (1 << 3);

    for(int i = 0; i < MAX_THREAD_NUM; i++) {
        if(pthread_mutex_init(&lock[i], NULL) != 0) {
             printf("pthread_mutex_init failed\n");
             exit(1);
        }
    }

    begintime = clock();
    //create four thread
    for(i = 0;i < MAX_THREAD_NUM; i++){
        printf("id[%d] = %d\n", i, id[i]);
        ret = pthread_create(&th[i], NULL, thread, &id[i]);

        if(ret != 0){
            printf("create thread failed\n");
            exit(1);
        }
    }


    endtime = clock();

    for(i = 0; i < MAX_THREAD_NUM; i++){
        pthread_join(th[i], NULL);
    }

    runtime  = endtime - begintime;
    printf("p-->%ld, q-->%ld, r-->%ld, s-->%ld\n", p, q, r, s);
    printf("running time : %ld ms \n", runtime);

    return 0;
}

void* thread(void* id)
{
    int num = *(int *)id;

    printf("num-------------->%d\n",num);
 
    int i = 0;   


    while(i < MAX_COUNT){
        if(num & (1 << 0)){    
            pthread_mutex_lock(&lock[0]);
            p++;
            pthread_mutex_unlock(&lock[0]);
         }

         if(num & (1 << 1)){    
            pthread_mutex_lock(&lock[1]);
            q++;
            pthread_mutex_unlock(&lock[1]);
         }


         if(num & (1 << 2)){    
            pthread_mutex_lock(&lock[2]);
            r++;
            pthread_mutex_unlock(&lock[2]);
         }

         if(num & (1 << 3)){
            pthread_mutex_lock(&lock[3]);
            s++;
            pthread_mutex_unlock(&lock[3]);
         }

         i++;
    }


}
