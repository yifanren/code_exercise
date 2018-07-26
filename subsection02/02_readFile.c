#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <locale.h>

#define LC_NAME_zh_CN_DEFAULT   LC_NAME_zh_CN_GBK


void show(char *arr[4]);
void sort(char *arr[4]);

int main(int argc,char *argv[])
{

    if(argc!=2){
        printf("please input the filename\n");
        exit(1);
    }


    FILE *fp;
    char *line[4];
    char buf[128];
    int i=0;



    fp=fopen(argv[1],"r");
    if(fp==NULL){
        printf("open failed\n");
        exit(0);
    }

    char ch;
    while((fgets(buf,128,fp))!=NULL)
    {
        printf("%s",buf);
        line[i]=(char *)malloc(strlen(buf)+1);
        memcpy(line[i],buf,strlen(buf)+1);

        //printf("line is %s\n",line[i]);
        i++;
        //bzero(buf,128);
    }

    printf("\n");
    printf("********the content is***********\n");
    show(line);
    sort(line);
    printf("********the content after sort with ASCII***********\n");
    show(line);

    fclose(fp);


    return 0;
}

//show the arr
void show(char *arr[4])
{
    int i;

    for(i=0;i<4;i++)
        printf("%s",arr[i]);

    printf("\n");
}

//have a sort
void sort(char *arr[4])
{
    int i,j;
    char newArr[128];
    char temp[128];
    char temp_se[128];
    
    char *localname=NULL;
    wchar_t *strzhFirst;
    wchar_t *strzhSecond;

        for(i=0;i<4-1;i++){
            for(j=i;j<4-1;j++){

                strzhFirst=arr[i];
                strzhSecond=arr[j+1];
               
                strzhFirst=setlocale(LC_ALL,LC_NAME_zh_CN_DEFAULT);
                strzhSecond=setlocale(LC_ALL,LC_NAME_zh_CN_DEFAULT);

                if((strcmp(temp,temp_se))>0){
                    strcpy(newArr,temp_se);
                    strcpy(temp_se,temp);
                    strcpy(temp,newArr);
                }
        }
    }
}

