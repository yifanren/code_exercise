#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>



int Compare(const void* p1, const void* p2)
{
    return -(strcmp(*(char**)p1, *(char**)p2));
}

int main(void)
{
    FILE *fp;
    char buf[128];
    int i;
    int num = 0;
    char *line[1000];

    fp = fopen("input.txt","r");
    if (fp == NULL) {
        printf("fopen failed\n");
        exit(1);
    } 

    while((fgets(buf,128,fp)) != NULL) {
        line[num] = (char *)malloc(strlen(buf)+1);
        memcpy(line[num], buf, strlen(buf)+1);

        num++;
        memset(buf, 0, 128);
    }

    fclose(fp);

    printf("********the content is***********\n");
    for (i = 0; i < num; i++) {
        printf("%s", line[i]);
    }
   
    qsort(line, num, sizeof(line[0]), Compare);

    printf("********the content after sort with ASCII***********\n");
    for (i = 0; i < num; i++) {
        printf("%s", line[i]);
    }

    for (i = 0; i < num; i++) {
        free(line[i]);
        line[i] = NULL;
    }

    return 0;
}
