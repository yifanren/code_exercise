#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

struct file{
    int num;
    off_t total;
    int fileCount;
};

void countFileNum(char * folderPath,struct file *p_info);

int main(int argc,char* argv[])
{
    if(argc != 2){
        printf("please input the folder path :\n");
        exit(1);
    }

    struct file *p_info;   
    p_info = (struct file *)malloc(sizeof(struct file));

    countFileNum(argv[1],p_info);

    printf("the tatal Bytes is %ld\n",p_info->total);
    printf("there are %d file more than 1M\n",p_info->num);
    printf("there are %d file\n",p_info->fileCount);

    free(p_info);
    return 0;
}

void countFileNum(char * folderPath,struct file  *p_info)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    char filename[128];

    //judge the path is file or folder
    //not folder
    if((dp = opendir(folderPath)) == NULL){
        printf("the path is not folder path\n");
        exit(1);    
    }

    //search all file in the folder
    while((entry = readdir(dp)) != NULL){
        sprintf(filename, "%s/%s", folderPath, entry->d_name);
        lstat(filename, &statbuf);

        //        if(S_ISDIR(statbuf.st_mode)){
        if( DT_DIR & entry->d_type){
            if(strcmp(".", entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
                continue;

            countFileNum(filename, p_info);
        }else{

            if((statbuf.st_size) > (1024*1024))
                p_info->num++;

            p_info->total += statbuf.st_size;
            p_info->fileCount += 1;
        }
    }

    closedir(dp);
    }
