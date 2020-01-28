# include "header.h"

void getIP(char *ip, int sz){
  // get server IP
  int n=0;
  bzero(ip, sz);
  printf("Enter server ip: ");
  while((ip[n++]=getchar())!='\n');
  ip[--n]='\0';
}

int createFd(void){
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd==-1){
    printf("Socket creation failed...\n");
    return -1;
  }
  else
    printf("Socket successfully created..\n");
  return sockfd;
}