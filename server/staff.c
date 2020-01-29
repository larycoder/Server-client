# include "header.h"

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

int addNewUser(int userFd, int* talkers, int* listeners){
  char mess;
  if(read(userFd, &mess, 1) < 1) return -1;

  // Decide connection is listen or talk
  switch (mess){
  case IAMTALKER:
    if(push2List(userFd, talkers, MAX_TALKER) < 0){
      return -1;
    }
    break;
  case IAMLISTENER:
    if(push2List(userFd, listeners, MAX_TALKER) < 0){
      return -1;
    }
    break;
  default:
    return -1;
    break;
  }
  return 0;
}

int push2List(int fd, int* list, int sz){
  for(int i = 0; i < sz; i++){
    if(list[i] == 0){
      list[i] = fd;
      return 0;
    }
  }
  return -1;
}