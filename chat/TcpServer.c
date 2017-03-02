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
#define LISTNUM 20

int main()
{
    int sockfd, confd;
    struct sockaddr_in addrServer, addrClient;
    socklen_t len;
    char buf[BUFFERSIZE];
    unsigned int port, listnum;
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd;

    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Server create socket error!");
        exit(errno);
    }
    else
        printf("Server socket created!\n");

    bzero(&addrServer, sizeof addrServer);
    addrServer.sin_family = AF_INET;
    addrServer.sin_addr.s_addr = htons(INADDR_ANY);
    addrServer.sin_port = htons(PORT);

    if((bind(sockfd, (struct sockaddr *)&addrServer, sizeof(struct sockaddr))) == -1)
    {
        perror("Server bind error!\n");
        exit(errno);
    }
    else
        printf("Bind success!\n");
    if(listen(sockfd, listnum) == -1)
    {
        perror("Server listen error!");
        exit(errno);
    }
    else
        printf("Server listen success!\n");

    while(1)
    {
        len = sizeof(struct sockaddr);
        if((confd = accept(sockfd, (struct sockaddr*)&addrClient, &len)) == -1)
        {
            perror("Server accept error");
            exit(errno);
        }
        else
        {
            printf("Connected to: %s:%d\n", inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port));
        }
        while(1)
        {
            FD_ZERO(&rfds);
            FD_SET(0, &rfds);
            maxfd = 0;
            FD_SET(confd, &rfds);
            if(maxfd < confd)
                maxfd = confd;
            tv.tv_sec = 6;
            tv.tv_usec = 0;
            retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
            if(retval == -1)
            {
                perror("Select error!");
                break;
            }
            else if(retval == 0)
            {
                printf("Server waiting!\n");
                continue;
            }
            else
            {
                if(FD_ISSET(0, &rfds))
                {
                    bzero(buf, sizeof buf);
                    fgets(buf, sizeof buf, stdin);
                    send(confd, buf, sizeof buf, 0);
                    if(strncasecmp(buf, "quit", 4) == 0)
                    {
                        printf("Quit!\n");
                        break;
                    }
                }
                if(FD_ISSET(confd, &rfds))
                {
                    bzero(buf, sizeof buf);
                    recv(confd, buf, sizeof buf, 0);
                    if(strncasecmp(buf, "quit", 4) == 0)
                        break;
                    printf("Recv:%s", buf);
                }
            }
        }
        close(confd);
        break;
    }
    close(sockfd);
    return 0;
}
