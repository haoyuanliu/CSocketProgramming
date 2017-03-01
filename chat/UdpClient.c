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

#define SERVER_PORT 6000
#define CLIENT_PORT 6001
#define IP "127.0.0.1"
#define BUFFERSIZE 100
