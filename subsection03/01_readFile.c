#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

int num=0;
off_t total=0;
int fileCount=0;

void IsFloder(char * folderPath);


//give the parameter into function
int main(int argc,char* argv[])
{
    if(argc!=2){
        printf("please input the folder path :");
    }

    IsFloder(argv[1]);

    printf("\n");
    printf("^^^^^^^^^^^^^^^^^^^^^the main info^^^^^^^^^^^^^^^^^^^^^\n");
    printf("the tatal Bytes is %d\n",total);
    printf("there are %d file more than 1M\n",num);
    printf("there are %d file\n",fileCount);

    return 0;
}



void IsFloder(char * folderPath)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    char filename[128];

    //    off_t total=0;
    //    int num=0;

    //judge the path is file or folder
    //not folder
    if((dp=opendir(folderPath))==NULL){
        printf("the path is not folder path\n");
        exit(1);    
    }

    //is folder,enter the folder
    //chdir(folderPath);



    //search all file in the folder
    while((entry=readdir(dp))!=NULL){
        lstat(entry->d_name,&statbuf);

        if(S_ISDIR(statbuf.st_mode)){
            printf("-------------this wen jian jia --------%s\n",entry->d_name);
            if(strcmp(".",entry->d_name)==0||strcmp("..",entry->d_name)==0)
                continue;

            // strcat(filename,"/");
            //strcat(filename,entry->d_name);
            sprintf(filename,"%s/%s",folderPath,entry->d_name);

            printf("***************%s*****************",filename);
            IsFloder(filename);

        }else{

            printf("-------------this is the file name--------%s\n",entry->d_name);

            if((statbuf.st_size)>(1024*1024))
                num++;

            total+=statbuf.st_size;
            fileCount+=1;
        }
    }

    //return the last item
    //chdir("..");

    closedir(dp);

}
#if 0


    if(entry->d_reclen==24){
        if(entry->d_type==8){

            printf("-------------this is the file name--------%s\n",entry->d_name);

            if((statbuf.st_size)>(1024*1024))
                num++;

            total+=statbuf.st_size;
            fileCount+=1;


        }else{

            printf("-------------this wen jian jia --------%s\n",entry->d_name);
            if(strcmp(".",entry->d_name)==0||strcmp("..",entry->d_name)==0)
                continue;

            // strcat(filename,"/");
            //strcat(filename,entry->d_name);
            sprintf(filename,"%s/%s",folderPath,entry->d_name);

            printf("***************%s*****************",filename);
            IsFloder(filename);

        }
    }

#endif


















