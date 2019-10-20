#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

// define maximum value of buffer
#define MAX 1024

// define port of listening socket
#define PORT 8080

// define name of type
#define SA struct sockaddr

// talker operation
void talk(char* room, int word_sz, int fd);
void hear(char* room, int word_sz, int fd);

// staff operation
int open_chat_room(struct sockaddr_in chat_room);