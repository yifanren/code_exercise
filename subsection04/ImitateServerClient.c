#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 6666
#define BUFSIZE 128

void client(char *ip);
void server();

int main(int argc,char *argv[])
{
    if(argc != 2){
        printf("please input server or client\n");
        exit(1);
    }

    if(!(strcmp(argv[1], "client"))){
        printf("this is client\n");
        printf("please input the server ip :");
        char ip[16];
        scanf("%s",ip);

        printf("%s\n",ip);
        client(ip);
    }

    if(!(strcmp(argv[1],"server"))){
        printf("this is server\n");
        server(); 
    }

    return 0;
}

void client(char *ip)
{
    int clifd;
    struct sockaddr_in remote_addr;
    char buf[BUFSIZE];
    int ret;

    memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = inet_addr(ip);
    remote_addr.sin_port = htons(PORT);

    if((clifd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("client socket failed\n");
        exit(1);
    }

    ret = connect(clifd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr));
    if(ret < 0){
        perror("connect failed\n");
        close(clifd);
        exit(1);
    }

    strcpy(buf, "ping"); 
    ret = write(clifd, buf, strlen(buf));
    if(ret == -1){
        perror("client send failed\n");
        close(clifd);
        exit(1);
    }

    memset(buf, 0, BUFSIZE);
    ret = read(clifd, buf, BUFSIZE);

    printf("%s\n",buf);

    close(clifd);

}

void server(){

    int serfd,clifd;
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t addrlen;
    char buf[BUFSIZE];
    int len;

    if((serfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("server socket failed\n");
        exit(1);
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if(bind(serfd, (struct sockaddr *)&server, sizeof(server)) == -1){
        perror("server bind failed\n");
        close(serfd);
        exit(1);
    }

    if(listen(serfd, 1) == -1){
        perror("server listen failed\n");
        close(serfd);
        exit(1);
    }

    addrlen = sizeof(client);
    if((clifd = accept(serfd, (struct sockaddr *)&client, &addrlen)) == -1){
        perror("server accept failed\n");
        close(serfd);
        exit(1);
    }

    //connect success
    printf("acccept client %s\n", inet_ntoa(client.sin_addr));

    memset(buf, 0, BUFSIZE);
    len = read(clifd, buf, BUFSIZE);
    if(len > 0)
        printf("%s\n", buf);

    memset(buf, 0, BUFSIZE);
    strcpy(buf, "pong");
    write(clifd, buf, strlen(buf));
    
    close(clifd);
    close(serfd);

}
