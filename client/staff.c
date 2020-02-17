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

// setup non-blocking reading from stdin
int setNonBlockingReading(int fd){
  // setup non-blocking reading for input
  if(fcntl(fd, F_SETFL, O_NONBLOCK) == 0){
    printf("Set non-blocking reading successful!\n");
    return 0;
  }
  else{
    printf("fail to set non-blocking reading successful!\n");
    return -1;
  }
}

int trackUserMess(char* buff, int sz, int current, int* enterFlag){
  char c;
  // ctrl D (^D) is 4
  read(STDIN_FILENO, &c, 1);
  if(c == '\n'){
    *enterFlag = 1;
  }
  // BackSpace is 10 and Del is 127
  else if((c == '\b' || c == 127) && current > 0){
    current--;
    buff[current] = '\0';
    return current;
  }
  buff[current] = c;
  buff[current + 1] = '\0';
  current = (current + 1) % sz;
  return current;
}