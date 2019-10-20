# include "header.h"

int open_chat_room(struct sockaddr_in chat_room){
  int fd = socket(AF_INET, SOCK_STREAM,0);
  if(fd<0){
    printf("failed opening socket...\n");
    exit(0);
  }

  chat_room.sin_family = AF_INET;
  chat_room.sin_addr.s_addr = htonl(INADDR_ANY);
  chat_room.sin_port = htons(PORT);

  int bs = bind(fd, (SA*)&chat_room, sizeof(chat_room));
  if(bs!=0){
    printf("failed binding socket...\n");
    exit(0);
  }

  return fd;
}