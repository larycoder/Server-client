#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd){
  char buff[MAX];
  int n;
  for(;;){
    bzero(buff, sizeof(buff));
    printf("Enter the string: ");
    n=0;
    while((buff[n++]=getchar())!='\n');
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("From Server: %s", buff);
    if((strncmp(buff,"exit",4))==0){
      printf("Client Exit...\n");
      break;
    }
  }
}

void getIP(char *ip){
  // get server IP
  int n=0;
  bzero(ip, sizeof(ip));
  printf("Enter server ip: ");
  while((ip[n++]=getchar())!='\n');
  ip[--n]='\0';
}

int main(){
  int sockfd, connfd;
  struct sockaddr_in servaddr, cli;

  // socket create and varification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd==-1){
    printf("Socket creation failed...\n");
    exit(0);
  }
  else
    printf("Socket successfully created..\n");
  bzero(&servaddr, sizeof(servaddr));

  char ip[30];
  getIP(ip);

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(ip);
  servaddr.sin_port = htons(PORT);

  // connect the client socket to server socket
  if(connect(sockfd, (SA*)&servaddr, sizeof(servaddr))!=0){
    printf("connection with the server failed...\n");
    exit(0);
  }
  else 
    printf("Connected to the Server..\n");

  // function for chat
  func(sockfd);

  // close the socket
  close(sockfd);
}