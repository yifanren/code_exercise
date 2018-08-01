#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <locale.h>

void release(char *ch[],int num);
void show(char *arr[], int num);
int sort(char *arr[], int num, int flag);
int cmpstringp(const void *p1, const void *p2);

int main(int argc,char *argv[])
{
    setlocale(LC_ALL, "zh_CN.utf8");

    if(argc != 2){
        printf("pleaes input the filename\n");
        exit(1);
    }

    FILE *fp;
    static int size;  
    char buf[128];
    int i = 0;

    fp = fopen(argv[1], "r");
    if(fp == NULL){
        printf("open failed\n");
        exit(0);
    }

    while((fgets(buf, 128, fp)) != NULL)
        size++;

    char ch;
    char *line[size];

    fclose(fp);

    fp = fopen(argv[1],"r");
    while((fgets(buf, 128, fp)) != NULL)
    {
        line[i] = (char *)malloc(strlen(buf)+1);
        memcpy(line[i], buf, strlen(buf)+1);
        i++;
        memset(buf,0,128);
    }

    printf("********the content is***********\n");
    show(line,size);
    //qsort(line, size, sizeof(line[0]), sort(line, size, 0));
    //qsort(line, size, sizeof(line[0]), strcoll);
    qsort(line, size, sizeof(line[0]), cmpstringp);
    //sort(line, size, 0);
    printf("********the content after sort with ASCII***********\n");
    show(line, size);

    fclose(fp);
    release(line, size);

    return 0;
}

//show the arr
void show(char *arr[], int num)
{
    int i;
    for(i = 0; i < num; i++)
        printf("%s",arr[i]);
}

    
int cmpstringp(const void *p1, const void *p2)
{
   // setlocale(LC_ALL, "zh_CN.utf8");
    return strcoll(*(char **)p1, *(char **)p2);
}

//free the space
void release(char *ch[], int num)
{
    int i;
    for(i = 0; i < num; i++)
        free(ch[i]);

}
