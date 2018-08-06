#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>

#define PORT            6666
#define BUFSIZE         128
#define MAX_TIMEOUT     10
#define MAX_CLIENT_CNT  50

typedef struct {
    int    fd;
    time_t startTime;
} ClientInfo_t;

ClientInfo_t client[MAX_CLIENT_CNT] = { {0, 0}, };

void* ThreadClientRecv(void *param) {

    int waitRecvCnt = 0, ret = 0;
    int maxFd = 0;
    struct timeval timeout = {0, 100 * 1000};
    char buf[BUFSIZE];
    struct timeval sponseTime[MAX_CLIENT_CNT];
    fd_set readFdSet;
    sleep(1);

    do {
        maxFd = 0;
        waitRecvCnt = 0;
        FD_ZERO(&readFdSet);
        for(int i = 0; i < MAX_CLIENT_CNT; i++) {
            if(client[i].fd > 0) {
                FD_SET(client[i].fd, &readFdSet);
                if(maxFd < client[i].fd) {
                    maxFd = client[i].fd;
                }
            }
        }

        timeout.tv_sec  = 0;
        timeout.tv_usec = 100 * 1000;
        ret = select(maxFd + 1, &readFdSet, NULL, NULL, &timeout);
        if(ret > 0) {

            for(int i = 0; i < MAX_CLIENT_CNT; i++) {
                if(client[i].fd > 0 && FD_ISSET(client[i].fd, &readFdSet)) {
                    memset(buf, 0, BUFSIZE);
                    ret = read(client[i].fd, buf, BUFSIZE);
                    printf("client[%d] recv = %s\n", i, buf);
                    gettimeofday(&sponseTime[i], NULL);
                }
            }
        }
        else if(ret == 0) {
        }
        else {
            printf("select() error. err = %s\n", strerror(errno));
        }

        for(int i = 0; i < MAX_CLIENT_CNT; i++) {
            if(client[i].fd > 0 && (time(0) - client[i].startTime) >= 10) {
                close(client[i].fd);
                client[i].fd = 0;
                printf("client : client %d close\n", i);
            }

            if(client[i].fd > 0) {
                waitRecvCnt++;
            }
        }
    } while(waitRecvCnt > 0);

    printf("client recv response spent %ld us\n",sponseTime[MAX_CLIENT_CNT - 1].tv_usec - sponseTime[0].tv_usec);
}

//client 
void* TcpClient(void *ip)
{
    struct sockaddr_in remote_addr;
    char buf[BUFSIZE];
    int ret, i = 0;

    memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = inet_addr((char*)ip);
    remote_addr.sin_port = htons(PORT);

    memset(client, 0, sizeof(client));
    pthread_t threadIdClientRecv;
    pthread_create(&threadIdClientRecv, NULL, ThreadClientRecv, NULL);

    for(i = 0; i < MAX_CLIENT_CNT; i++) {
        int fd = 0;
        if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            perror("client socket failed\n");
            exit(1);
        }

        ret = connect(fd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr));
        if(ret < 0){
            perror("connect failed\n");
            close(client[i].fd);
            client[i].fd = 0;
            continue;
        }

        client[i].fd = fd;
        client[i].startTime = time(0);
        strcpy(buf, "ping"); 
        ret = write(client[i].fd, buf, strlen(buf));
        if(ret == -1){
            perror("client send failed\n");
            close(client[i].fd);
            client[i].fd = 0;
            continue;
        }
    }

    pthread_join(threadIdClientRecv, NULL);
}

//server
void* TcpServer(void *msg) 
{
    int serfd,connectfd;
    struct sockaddr_in serAddr;
    struct sockaddr_in cliAddr;
    socklen_t addrlen;
    char buf[BUFSIZE];
    int len;

    int clientFd[1000];

    if((serfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("server socket failed\n");
        exit(1);
    }

    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = INADDR_ANY;
    serAddr.sin_port = htons(PORT);

    if(bind(serfd, (struct sockaddr *)&serAddr, sizeof(serAddr)) == -1){
        perror("server bind failed\n");
        close(serfd);
        exit(1);
    }

    if(listen(serfd, 10) == -1){
        perror("server listen failed\n");
        close(serfd);
        exit(1);
    }

    int maxfd;
    //create select

    int connectAll[50];
    int num = 0;
    int i;
    pthread_t th; 
    
    addrlen = sizeof(client);
    fd_set readfd, rset;
    maxfd=serfd;
    FD_ZERO(&readfd);
    FD_SET(serfd, &readfd);

    while(1){

        rset = readfd;

        int nread = select(maxfd + 1, &rset, NULL, NULL, NULL); 
        if(nread < 0){
            perror("select failed\n");
            continue;
        }

        if(FD_ISSET(serfd, &rset)){

            if((connectfd = accept(serfd, (struct sockaddr *)&cliAddr, &addrlen)) == -1){
                perror("server accept failed\n");
                continue;
            }

            connectAll[num] = connectfd;
            FD_SET(connectAll[num], &readfd);
            if(connectfd > maxfd)
                maxfd = connectfd;

            num++;
            
            memset(buf, 0, BUFSIZE);
            len = recv(connectfd, buf, BUFSIZE, 0);
                
            if(len > 0){
                printf("server : recv client[%d]  %s\n",connectfd, buf);
                memset(buf, 0, BUFSIZE);
                strcpy(buf, "pong");
                write(connectfd, buf, strlen(buf) + 1);
            }
        }

        for(i = 0; i < num; i++){
            if(FD_ISSET(connectAll[i], &rset)){

                memset(buf, 0, BUFSIZE);
                len = recv(connectAll[i], buf, BUFSIZE, 0);
            
                if(len == 0){
                    close(connectAll[i]);
                }
                FD_CLR(connectAll[i], &readfd);
            }
        }
    }
    close(serfd);
}

int main(int argc,char *argv[])
{
    if (argc != 2) {
        printf("please input ip address :\n");
        exit(1);
    }

    pthread_t serThread, cliThread;
    pthread_create(&serThread, NULL, TcpClient, argv[1]);
    pthread_create(&cliThread, NULL, TcpServer, NULL);

    pthread_join(cliThread, NULL);
    pthread_join(serThread, NULL);

    return 0;
}
