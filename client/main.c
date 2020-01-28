# include "header.h"

void func(int sockfd){
  char buff[MAX];
  int n;
  for(;;){
    bzero(buff, sizeof(buff));
    printf("Enter the string: ");
    n=0;
    while((buff[n++]=getchar())!='\n');
    buff[n]='\0';
    write(sockfd, buff, strlen(buff));
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("From Server: %s", buff);
    if((strncmp(buff,"exit",4))==0){
      printf("Client Exit...\n");
      break;
    }
  }
}

int main(){
  int sockfd, connfd;
  struct sockaddr_in servaddr, cli;

  // socket fd create
  sockfd = createFd();
  if(sockfd == -1) exit(1);
  // get ip from user
  char ip[30];
  getIP(ip, sizeof(ip));

  // assign IP, PORT
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(ip);
  servaddr.sin_port = htons(PORT);

  // connect the client socket to server socket
  if(connect(sockfd, (SA*)&servaddr, sizeof(servaddr))!=0){
    printf("connection with the server failed...\n");
    exit(1);
  }
  else 
    printf("Connected to the Server..\n");

  // server connected in sockfd, do something !

  // function for chat
  func(sockfd);

  // close the socket
  close(sockfd);
}
