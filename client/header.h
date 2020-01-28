#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SA struct sockaddr
#define MAX 9000 // define max length of buffer
#define PORT 8080

// staff operation
void getIP(char *ip, int sz);
int createFd(void);