#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void show(char *arr[],int num);
void sort(char *arr[],int num);
void release(char *ch[],int num);

int main(int argc,char *argv[])
{

    if(argc != 2){
        printf("pleaes input the filename\n");
        exit(1);
    }

    FILE *fp;
    static int size;  
    char buf[128];
    int i=0;



    fp=fopen(argv[1],"r");
    if(fp==NULL){
        printf("open failed\n");
        exit(0);
    }

    while((fgets(buf,128,fp))!=NULL)
        size++;
        

    char ch;
    char *line[size];
   
    fclose(fp);

    fp=fopen(argv[1],"r");
    while((fgets(buf,128,fp))!=NULL)
    {
        line[i]=(char *)malloc(strlen(buf)+1);
        memcpy(line[i],buf,strlen(buf)+1);

        i++;
        memset(buf,0,128);
    }

    printf("********the content is***********\n");
    show(line,size);
    sort(line,size);
    printf("********the content after sort with ASCII***********\n");
    show(line,size);

    fclose(fp);
    release(line,size);


    return 0;
}

//show the arr
void show(char *arr[],int num)
{
    int i;

    for(i=0;i<num;i++)
        printf("%s",arr[i]);

    printf("\n");
}

//have a sort
void sort(char *arr[],int num)
{
    int i,j;
    char *newArr;

    for(i=0;i<num-1;i++){
        for(j=i;j<num-1;j++){
            if((strcmp(arr[i],arr[j+1]))<0){

                newArr=arr[j+1];
                arr[j+1]=arr[i];
                arr[i]=newArr;
           
            }
        }
    }
}


//free the space
void release(char *ch[],int num)
{
    int i;
    for(i=0;i<num;i++)
        free(ch[i]);

}
