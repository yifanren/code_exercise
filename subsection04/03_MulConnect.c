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

#define PORT 6776
#define BUFSIZE 128
#define MAX_THREAD_NUM 10

void TcpClient(char *ip);
void server();

int main(int argc,char *argv[])
{
    if(argc != 2 && argc != 3 ){
        printf("please input server or client+ip\n");
        exit(1);
    }

    if(argc == 2 ){
        if(!(strcmp(argv[1],"server"))){
            printf("this is server\n");
            server(); 
        }
    }

    if(argc == 3){
        if(!(strcmp(argv[1], "client"))){
            printf("this is client\n");
            TcpClient(argv[2]);
        }
    }

    return 0;
}

#define MAX_TIMEOUT     10
#define MAX_CLIENT_CNT  500

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
                printf("clent[%d] close\n", i);
            }

            if(client[i].fd > 0) {
                waitRecvCnt++;
            }
        }
        
    } while(waitRecvCnt > 0);

}

//client 
void TcpClient(char *ip)
{
    struct sockaddr_in remote_addr;
    char buf[BUFSIZE];
    int ret, i = 0;

    memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = inet_addr(ip);
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
void server(){

    int listenfd,connectfd;
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t addrlen;
    char buf[BUFSIZE];
    int len;

    int clientFd[1000];

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("server socket failed\n");
        exit(1);
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if(bind(listenfd, (struct sockaddr *)&server, sizeof(server)) == -1){
        perror("server bind failed\n");
        close(listenfd);
        exit(1);
    }

    if(listen(listenfd, 10) == -1){
        perror("server listen failed\n");
        close(listenfd);
        exit(1);
    }

    int maxfd;
    //create select

    fd_set readfd, rset;
    maxfd=listenfd;
    addrlen = sizeof(client);
    FD_ZERO(&readfd);
    FD_SET(listenfd, &readfd);

    while(1){
  
        rset = readfd;

        int nread = select(maxfd + 1, &rset, NULL, NULL, NULL); 
        if(nread < 0){
            perror("select failed\n");
            continue;
        }

        if(FD_ISSET(listenfd, &rset)){

            if((connectfd = accept(listenfd, (struct sockaddr *)&client, &addrlen)) == -1){
                perror("server accept failed\n");
                close(listenfd);
                exit(1);
            }

            FD_SET(connectfd, &readfd);
            if(connectfd > maxfd)
                maxfd = connectfd;

            //connect success
            printf("acccept client %s, fd is %d\n", inet_ntoa(client.sin_addr), connectfd);
            memset(buf, 0, BUFSIZE);
            len = recv(connectfd, buf, BUFSIZE, 0);
            if(len > 0){
                printf("recv-----fd : %d ------------%s\n", connectfd, buf);

            memset(buf, 0, BUFSIZE);
            strcpy(buf, "pong");
            write(connectfd, buf, strlen(buf));
            printf("send-----fd : %d ------------%s\n", connectfd, buf);
            }

            if(len == 0){
                printf("%d connect close\n",connect);
                close(connect);
            }
        }
         
        for(int i = 0; i < 1000; i++) {
            if(clientFd[i] == 0) {
                clientFd[i] = connectfd;
            }
        }
    }        

    close(connectfd);
    close(listenfd);
}
