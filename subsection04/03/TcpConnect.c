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
#include <sys/epoll.h>

#define PORT            6666
#define BUFSIZE         128
#define MAX_TIMEOUT     10
#define MAX_CLIENT_CNT  500

typedef struct {
    int    fd;
    time_t startTime;
} ClientInfo_t;

ClientInfo_t client[MAX_CLIENT_CNT] = { {0, 0}, };

static int serverClose;

void* ThreadClientRecv(void *param) 
{
    int waitRecvCnt, ret;
    int maxFd;
    char buf[BUFSIZE];
    fd_set readFdSet;
    int numReceiverMsg = 0;
    struct timeval beginTime, endTime;
    struct timeval timeout;
    
    usleep(10 * 1000); 

    gettimeofday(&beginTime, NULL);
    do {
        maxFd = 0;
        waitRecvCnt = 0;
        FD_ZERO(&readFdSet);
        for (int i = 0; i < MAX_CLIENT_CNT; i++) {
            if (client[i].fd > 0) {
                FD_SET(client[i].fd, &readFdSet);
           
            if (maxFd < client[i].fd) 
                maxFd = client[i].fd;
            }
        }

        timeout.tv_sec = 0;
        timeout.tv_usec = 500*1000;
        ret = select(maxFd + 1, &readFdSet, NULL, NULL, &timeout);
        if(ret > 0) {
        
            for (int i = 0; i < MAX_CLIENT_CNT; i++) {
               if (client[i].fd > 0 && FD_ISSET(client[i].fd, &readFdSet)) {
                    memset(buf, 0, BUFSIZE);
                    ret = recv(client[i].fd, buf, BUFSIZE, 0);
                    
                    if ((numReceiverMsg++) == (MAX_CLIENT_CNT - 1)) 
                        gettimeofday(&endTime, NULL);   
                }
            }
        }
        else if (ret < 0) {
            printf("select() error. err = %s\n", strerror(errno));
            continue;         
        }

        for (int i = 0; i < MAX_CLIENT_CNT; i++) {
            if (client[i].fd > 0 && (time(0) - client[i].startTime) >= 10) {
                close(client[i].fd);
                client[i].fd = 0;
            }

            if (client[i].fd > 0) {
                waitRecvCnt++;
            }
        }
    } while(waitRecvCnt > 0);
    printf("all client recv response spent %ld ms\n",
           (endTime.tv_sec*1000 + endTime.tv_usec/1000) -
           (beginTime.tv_sec*1000 + beginTime.tv_usec/1000));
}

//client 
void* TcpClient(void *args)
{
    struct sockaddr_in remoteAddr;
    char buf[BUFSIZE];
    int ret, i = 0;

    memset(&remoteAddr, 0, sizeof(remoteAddr));
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    remoteAddr.sin_port = htons(PORT);

    memset(client, 0, sizeof(client));
    pthread_t threadIdClientRecv;
    pthread_create(&threadIdClientRecv, NULL, ThreadClientRecv, NULL);

    for (i = 0; i < MAX_CLIENT_CNT; i++) {
        int fd = 0;
        if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("client socket failed\n");
            exit(1);
        }

        ret = connect(fd, (struct sockaddr *)&remoteAddr, sizeof(struct sockaddr));
        if (ret < 0) {
            printf("connect failed, err = %s\n", strerror(errno));
            close(client[i].fd);
            client[i].fd = 0;
            continue;
        }

        client[i].startTime = time(0);
        client[i].fd = fd;
        strcpy(buf, "ping"); 
        ret = send(client[i].fd, buf, strlen(buf) + 1, 0);
        if (ret == -1) {
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
    int  i;
    int timeout = 300;
    
    struct epoll_event events[MAX_CLIENT_CNT];
    struct epoll_event ev;
   
    int epollfd = epoll_create(MAX_CLIENT_CNT);
    if (epollfd == -1) {
        printf("epoll_create failed\n");
        exit(1);
    }
   
    if ((serfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("server socket failed\n");
        close(epollfd);
        exit(1);
    }

    ev.data.fd = serfd;
    ev.events = EPOLLIN;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, serfd, &ev);

    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serAddr.sin_port = htons(PORT);

    if (bind(serfd, (struct sockaddr *)&serAddr, sizeof(serAddr)) == -1) {
        perror("server bind failed\n");
        close(epollfd);
        close(serfd);
        exit(1);
    }

    if (listen(serfd, 10) == -1) {
        perror("server listen failed\n");
        close(epollfd);
        close(serfd);
        exit(1);
    }

    while(serverClose) {
        //wait event happen
        int nfds = epoll_wait(epollfd, events, MAX_CLIENT_CNT, timeout);
       
        if (nfds < 0) {
            printf("epoll failed\n");
            break;
        } else if (nfds == 0) {
           continue; 
        }
       
        for (i = 0; i < nfds; i++) { 
            //the new client connect
            int sockfd = events[i].data.fd;
            if (sockfd == serfd) {
                addrlen = sizeof(cliAddr);
                connectfd = accept(sockfd, (struct sockaddr*)&cliAddr, &addrlen);
                if (connectfd == -1) 
                    continue;   

                ev.data.fd = connectfd;
                ev.events = EPOLLIN;
                epoll_ctl(epollfd, EPOLL_CTL_ADD, connectfd, &ev);
            } else if (events[i].events & EPOLLIN) {
                int len = recv(sockfd, buf, BUFSIZE, 0);
                if (len > 0) {
                    memset(buf, 0, BUFSIZE);
                    strcpy(buf, "pong");
                    send(sockfd, buf, strlen(buf) + 1, 0);
                } else {
                    close(sockfd);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, NULL);
                }

            }
        }
    }

    close(epollfd);
    close(serfd);
}

int main(int argc,char *argv[])
{
    pthread_t serThread, cliThread;
    
    serverClose = 1;
    pthread_create(&serThread, NULL, TcpServer, NULL);
    sleep(1);
    pthread_create(&cliThread, NULL, TcpClient, NULL);
    
    pthread_join(cliThread, NULL);
    serverClose = 0;
    pthread_join(serThread, NULL);
    
    return 0;
}
