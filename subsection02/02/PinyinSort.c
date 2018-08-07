#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <locale.h>

#define BUFSIZE 128
#define LINECOUNT 1000

int CompareString(const void *p1, const void *p2)
{
    return strcoll(*(char **)p1, *(char **)p2);
}

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("pleaes input the filename\n");
        exit(1);
    }

    FILE *fp;
    int  i, num = 0;
    char buf[BUFSIZE];
    char *line[LINECOUNT];

    fp = fopen(argv[1],"r");
    if (fp == NULL) {
        printf("fopen failed\n");
        exit(1);
    }

    while ((fgets(buf, BUFSIZE, fp)) != NULL) {
        line[num] = (char *)malloc(strlen(buf) + 1);
        memcpy(line[num], buf, strlen(buf) + 1);
        
        num++;
    }

    fclose(fp);

    printf("********the content is***********\n");
    for (i = 0; i < num; i++)
        printf("%s",line[i]);
   
    char* curLocale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "zh_CN.utf8");
    qsort(line, num, sizeof(line[0]), CompareString);
    setlocale(LC_ALL, curLocale);
    printf("********the content after sort with ASCII***********\n");
    for (i = 0; i < num; i++)
        printf("%s",line[i]);
    
    for (i = 0; i < num; i++)
        free(line[i]);

    return 0;
}
    
