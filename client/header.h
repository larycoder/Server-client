#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <termios.h> // lib handle terminal

// lib serve multiple thread
#include <pthread.h>
#include <semaphore.h>

#include<fcntl.h> // Non-blocking I/O lib

#define SA struct sockaddr
#define MAX 9000 // define max length of buffer
#define R_PORT 80
#define W_PORT 80

// staff operation
void getIP(char *ip, int sz);
int createFd(void);
int trackUserMess(char* buff, int sz, int current);
int setNonBlockingReading(int fd);

/** 
* user interface handler
**/

void enableRawMode();
// 3 functions below need to work together
void drawUI(void);
void updateUserMess(char* messEnter);
void updateChatRoom(char* newMessCome);
