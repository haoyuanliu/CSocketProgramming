#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/time.h>

#define BUFFERSIZE 1024
#define PORT 6000

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in addrServer;
    socklen_t len;
    unsigned int port;
    char buf[BUFFERSIZE];
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Client create socket error!");
        exit(errno);
    }
    else
        printf("Client create socket success!\n");

    bzero(&addrServer, sizeof addrServer);
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(PORT);
    if(inet_aton(argv[1], (struct in_addr *)&addrServer.sin_addr.s_addr) == 0)
    {
        perror("argv[1]");
        exit(errno);
    }

    if(connect(sockfd, (struct sockaddr *)&addrServer, sizeof addrServer) == -1)
    {
        perror("Connect error!");
        exit(errno);
    }
    else
        printf("Connect success to %s:%d\n", argv[1], PORT);

    while(1)
    {
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        maxfd = 0;
        FD_SET(sockfd, &rfds);
        if(maxfd < sockfd)
            maxfd = sockfd;
        tv.tv_sec = 6;
        tv.tv_usec = 0;
        retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
        if(retval == -1)
        {
            printf("Select error!\n");
            break;
        }
        else if(retval == 0)
        {
            printf("Waiting!\n");
            continue;
        }
        else
        {
            if(FD_ISSET(sockfd, &rfds))
            {
                bzero(buf, BUFFERSIZE);
                recv(sockfd, buf, sizeof buf, 0);
                if(strncasecmp(buf, "quit", 4) == 0)
                    break;
                printf("Recv: %s", buf);
            }
            if(FD_ISSET(0, &rfds))
            {
                bzero(buf, sizeof buf);
                fgets(buf, sizeof buf, stdin);
                send(sockfd, buf, sizeof buf, 0);
                if(strncasecmp(buf, "quit", 4) == 0)
                {
                    printf("Quit!\n");
                    break;
                }
            }
        }
    }
    close(sockfd);
    return 0;
}
