#include <stdio.h>
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

int GetNetworkInfo(char *devName)
{
    int fd;
    fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd < 0) {
        printf("socket failed\n");
        return 0;
    }

    struct ifreq ifr;
    unsigned char mac[6];
    unsigned long ip, netmask, broadIp;

    //mac addr
    strcpy(ifr.ifr_name, devName);

    if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
        return 0;
    }

    memcpy(mac, ifr.ifr_hwaddr.sa_data, sizeof(mac));
    printf("MAC: %02x-%02x-%02x-%02x-%02x-%02x\n",
            mac[0],mac[1], mac[2], mac[3], mac[4],mac[5]);

    //Ip
    if (ioctl(fd, SIOCGIFADDR, &ifr) < 0) {
        ip = 0;
    } else {
        ip = *(unsigned long *)&ifr.ifr_broadaddr.sa_data[2];
    }

    printf("IP: %s\n", inet_ntoa(*(struct in_addr*)&ip));

    //broadIp
    if (ioctl(fd, SIOCGIFBRDADDR, &ifr) < 0) {
        broadIp = 0;
    } else {
        broadIp = *(unsigned long*)&ifr.ifr_broadaddr.sa_data[2];
    }

    printf("BroadIP: %s\n", inet_ntoa(*(struct in_addr*)&broadIp));

    //mask addr
    if (ioctl(fd, SIOCGIFNETMASK, &ifr) < 0) {
        netmask = 0;
    } else {
        netmask = *(unsigned long*)&ifr.ifr_netmask.sa_data[2];
    }

    printf("Netmask: %s\n", inet_ntoa(*(struct in_addr*)&netmask));

    close(fd);
}

int main(int argc,char *argv[])
{

    if (argc != 2) {
        perror("pleast input the interface name\n");
        exit(1);
    }

    GetNetworkInfo(argv[1]);

    return 0;
}




