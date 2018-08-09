#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

struct CountsInfo_s {
    off_t  totalSize;
    int    fileCount;
};

void CountFileNum(char* folderPath, struct CountsInfo_s* fileInfo)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    char filename[1024];

    //judge the path is file or folder
    if ((dp = opendir(folderPath)) == NULL) {
        printf("the path is not folder path\n");
        exit(1);    
    }

    //search all file in the folder
    while ((entry = readdir(dp)) != NULL) {
        sprintf(filename, "%s/%s", folderPath, entry->d_name);
        lstat(filename, &statbuf);

        if (DT_DIR & entry->d_type) {
            if (strcmp(".", entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
                continue;

            CountFileNum(filename, fileInfo);
        } else {
            if ((statbuf.st_size) > (1024*1024))
                fileInfo->fileCount += 1;

            fileInfo->totalSize += statbuf.st_size;
        }
    }
    closedir(dp);
}

int main(int argc,char* argv[])
{
    if(argc != 2){
        printf("please input the folder path :\n");
        exit(1);
    }

    struct CountsInfo_s fileInfo;

    fileInfo.totalSize = 0;
    fileInfo.fileCount = 0;
    CountFileNum(argv[1], &fileInfo);

    printf("the tatal Bytes is %ld\n",fileInfo.totalSize);
    printf("there are %d file more than 1M\n",fileInfo.fileCount);
   
    return 0;
}

