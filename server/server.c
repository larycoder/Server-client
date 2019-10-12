#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

int reply(int sockfd){
  char buff[MAX];
  bzero(buff, MAX);
  read(sockfd, buff, MAX);
  printf("client: %s\n", buff);
  if(strncmp("exit", buff, 4)==0);
  bzero(buff, MAX);
  strncpy(buff, "exit\b\b\b\bok,got it\n",MAX);
  write(sockfd, buff, MAX);
}

// Driver function
int main(){
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;

  // socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd==-1){
    printf("socket creation failed...\n");
    exit(0);
  }
  else
    printf("Socket successfully created..\n");
  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  // Binding newly created socket to given IP and verification
  if((bind(sockfd, (SA*)&servaddr, sizeof(servaddr)))!=0){
    printf("socket bind failed...\n");
    exit(0);
  }
  else{
    printf("Socket successfully binded..\n");
  }

  // Now server is ready to listen and verification
  if((listen(sockfd, 5))!=0){
    printf("Listen failed...\n");
    exit(0);
  }
  else 
    printf("Server listening..\n");

  // Accept the data packet from client and verification
  while(1){
    len = sizeof(cli);
    pid_t pid=0;
    connfd = accept(sockfd, (SA*)&cli, &len);
    if(connfd < 0){
      printf("server accept failed...\n");
    }
    else{ 
      pid=fork();
    }
    if(pid==0){
      printf("server accept the client at child process: %d\n", getpid());
      reply(connfd);
      shutdown(connfd, SHUT_RDWR);
      close(connfd);
    }
    else close(connfd);
  }
  return 0;
}