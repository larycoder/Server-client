#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

// define maximum value of buffer
#define MAX 1024

// define port of listening socket
#define PORT 8080

// define max client can connect at same time
#define MAX_TALKER 10

// define name of type
typedef struct sockaddr SA;

// talker operation
int talk(char* room, int word_sz, int fd);
int hear(char* room, int word_sz, int fd);
void broad_cast(char* room, int word_sz, int* talker_array);

// staff operation
int createFd(void);
int push2List(int fd, int* list, int sz);
int addNewUser(int userFd, int* talkers, int* listeners);

/*
  * Define of protocol
*/
#define IAMTALKER '0'
#define IAMLISTENER '1'