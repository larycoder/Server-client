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
    return -1;
  }

  return fd;
}

void register_talker(int lfd, int* flag, fd_set* readfds, int* max_fd, int* talker_list, int* talker_index, sem_t* mutex){
  // this is atomic work
  sem_wait(mutex);
  FD_ZERO(readfds);
  FD_SET(lfd, readfds);
  *max_fd = lfd;

  int check;
  check = listen(lfd, 5);
  if(check != 0){
    printf("listen failed...\n");
    *flag = CLOSE_FLAG;
    exit(0);
  }
  printf("Server listenning...\n");
  sem_post(mutex);

  while(1){
    // accept new talker to chat room
    int talker = accept(lfd, (SA*)NULL, NULL);

    // this is atomic work
    sem_wait(mutex);
    printf("accept new talker at fd %d\n", talker);
    talker_list[*talker_index] = talker;
    *talker_index+=1;
    // add talker to specify list for "select"
    FD_SET(talker, readfds);
    if(talker > *max_fd) *max_fd = talker;
    sem_post(mutex);

    // stop register when get close flag
    if(*flag == CLOSE_FLAG) return;
  }
}