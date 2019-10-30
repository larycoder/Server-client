#include "header.h"

int main(){
  // prepare for listen socket
  int opt = 1;
  int talker_list[MAX_TALKER];
  char room[MAX];
  bzero(talker_list, MAX_TALKER);
  bzero(room, MAX);
  // open listen socket
  struct sockaddr_in Room_addr;
  int Room_fd = open_chat_room(Room_addr);
  // allow reuse of local addresses for this addr
  // setsockopt(Room_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
  if(listen(Room_fd, 5)<0){
    printf("listen failed...\n");
    exit(0);
  }
  printf("server listenning...\n");
  // prepare for select
  int max_fd, activity, valread;
  fd_set readfds;
  // server real running time
  while(1){
    FD_ZERO(&readfds);
    // add listen socket to specify list
    FD_SET(Room_fd, &readfds);
    max_fd = Room_fd;
    // add talker socket to specify list
    for(int i=0; i < MAX_TALKER; i++){
      if(talker_list[i] != 0){
        FD_SET(talker_list[i], &readfds);
        if(talker_list[i] > max_fd) max_fd = talker_list[i];
      }
    }
    // wait for the activity
    activity = select(max_fd+1, &readfds, NULL, NULL, NULL);
    if(activity == -1){
      printf("Select occurred error...\n");
      exit(0);
    }
    if(FD_ISSET(Room_fd, &readfds)){
      int talker = accept(Room_fd, (SA*)NULL, NULL);
      printf("Accept new talker at fd %d\n", talker);
      for(int i=0; i<MAX_TALKER; i++){
        if(talker_list[i] == 0){
          talker_list[i] = talker;
          break;
        }
      }
    }
    for(int i=0; i<MAX_TALKER; i++){
      if(talker_list[i] != 0 && FD_ISSET(talker_list[i], &readfds)){
        valread = talk(room, sizeof(room), talker_list[i]);
        if(valread <= 0 || valread >= (MAX-1)){
          printf("fd %d close\n", talker_list[i]);
          close(talker_list[i]);
          talker_list[i] = 0;
        }
        else{
          room[++valread]='\0';
          printf("client [%d] talk: %s\n", i, room);
          broad_cast(room, strlen(room), talker_list);
          printf("finishing broad casting\n");
        }
      }
    }
  }
  return 0;
}
