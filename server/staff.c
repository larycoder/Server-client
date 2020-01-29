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

int resetQueue(fd_set* fdSet, int* list, int sz){
  FD_ZERO(fdSet);
  int max = -1;
  for(int i = 0; i < sz; i++){
    if(list[i] != 0){
      FD_SET(list[i], fdSet);
      if(max < list[i] || max < 0){
        max = list[i];
      }
    }
  }
  return max + 1;
}

int add2Queue(fd_set* fdSet, int* list, int sz, int nfds){
  int max = nfds - 1;
  for(int i = 0; i < sz; i++){
    if(list[i] != 0){
      FD_SET(list[i], fdSet);
      if(max < list[i]) max = list[i];
    }
  }
  return max + 1;
}