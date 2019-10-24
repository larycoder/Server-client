#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// define maximum value of buffer
#define MAX 1024

// define port of listening socket
#define PORT 8080

// define max client can connect at same time
#define MAX_TALKER 10

// define name of type
typedef struct sockaddr SA;

// signal flag of server
#define CLOSE_FLAG -1

// talker operation
void talk(char* room, int word_sz, int fd);
void hear(char* room, int word_sz, int fd);
void broad_cast(char* room, int word_sz, int* talker_array, int num_talker);

// staff operation
int open_chat_room(struct sockaddr_in chat_room);
void register_talker(int lfd, int* flag, fd_set* readfds, int* max_fd, int* talker_list, int* talker_index, sem_t* mutex);