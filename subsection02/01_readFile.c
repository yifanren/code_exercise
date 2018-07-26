#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>


void show(char *arr[],int num);
void sort(char *arr[],int num);
void refree(char *ch[],int num);

int main(void)
{
    
    FILE *fp;
    static int size=0;
    
    char buf[128];
    int i=0;



    fp=fopen("input.txt","r");
    if(fp==NULL){
        printf("open failed\n");
        exit(0);
    }

    while((fgets(buf,128,fp))!=NULL)
        size++;
        
    printf("%d\n",size);

    char ch;
    char *line[size];
   
   fclose(fp);

  fp=fopen("input.txt","r"); 
    while((fgets(buf,128,fp))!=NULL)
    {
        printf("%s",buf);

        line[i]=(char *)malloc(strlen(buf)+1);
        printf("line[%d] = %p, len = %d\n", i, line[i], strlen(buf));
        memcpy(line[i],buf,strlen(buf)+1);

        //printf("line is %s\n",line[i]);
        i++;
        //bzero(buf,128);
    }

    printf("********the content is***********\n");
    show(line,size);
    sort(line,size);
    printf("********the content after sort with ASCII***********\n");
    show(line,size);

    fclose(fp);
    refree(line,size);


    return 0;
}

//show the arr
void show(char *arr[],int num)
{
    int i;

    for(i=0;i<num;i++)
        printf("%s",arr[i]);

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
void refree(char *ch[],int num)
{
    int i;
    for(i=0;i<num;i++)
        free(ch[i]);

}
