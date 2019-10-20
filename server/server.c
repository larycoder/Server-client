#include "header.h"

// Driver function
int main(){
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;

  sockfd = open_chat_room(servaddr);
  
  if((listen(sockfd, 5))!=0){
    printf("Listen failed...\n");
    exit(0);
  }
  else 
    printf("Server listening..\n");
  len = sizeof(cli);
  connfd = accept(sockfd, (SA*)&cli, &len);
  return 0;
}