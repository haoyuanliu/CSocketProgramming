#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<sys/errno.h>

#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

#include<pthread.h>
#include<time.h>

#define RECV_PORT 6001
#define SEND_PORT 6000
#define IP "127.0.0.1"
#define BUFFERSIZE 100

char sendbuf[BUFFERSIZE];
char recvbuf[BUFFERSIZE];
time_t lt;

void *threadSend(void *fd)
{
    int sockSend = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addrSend;
    bzero(&addrSend, sizeof addrSend);
    addrSend.sin_family = AF_INET;
    addrSend.sin_addr.s_addr = inet_addr(IP);
    addrSend.sin_port = htons(SEND_PORT);
    connect(sockSend, (struct sockaddr *)&addrSend, sizeof addrSend);
    bzero(sendbuf, sizeof sendbuf);
    while(fgets(sendbuf, sizeof sendbuf, stdin) != 0)
    {
        send(sockSend, sendbuf, sizeof sendbuf, 0);
        bzero(sendbuf, sizeof sendbuf);
    }
    exit(0);
    free(fd);
    pthread_exit(NULL);
}

void *threadRecv(void *fd)
{
    int sockRecv = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addrRecv;
    bzero(&addrRecv, sizeof addrRecv);
    addrRecv.sin_family = AF_INET;
    addrRecv.sin_addr.s_addr = INADDR_ANY;
    addrRecv.sin_port = htons(RECV_PORT);
    bind(sockRecv, (struct sockaddr *)&addrRecv, sizeof addrRecv);
    listen(sockRecv, 5);
    int connFd = accept(sockRecv, NULL, NULL);
    bzero(recvbuf, sizeof recvbuf);
    while(1)
    {
        recv(connFd, recvbuf, sizeof recvbuf, 0);
        printf("Recv:%s", recvbuf);
        printf("..........");
        printf(ctime(&lt), stdout);
        bzero(recvbuf, sizeof recvbuf);
    }
    exit(0);
    free(fd);
    pthread_exit(NULL);
}

int main()
{
    lt = time(NULL);
    pthread_t pthread_recv;
    int fd_recv = pthread_create(&pthread_recv, NULL, threadRecv, NULL);
    if(fd_recv != 0)
        perror("Thread recv creat error!");

    pthread_t pthread_send;
    int fd_send = pthread_create(&pthread_send, NULL, threadSend, NULL);
    if(fd_send != 0)
        perror("Thread sent creat error!");



    while(1) { }

    exit(0);
    return EXIT_SUCCESS;
}
