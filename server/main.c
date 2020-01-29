#include "header.h"

int main(){
  // prepare listenning socket
  int Room_fd = createFd();
  if(Room_fd < 0) exit(1);

  struct sockaddr_in Room_addr;
  Room_addr.sin_family = AF_INET;
  Room_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  Room_addr.sin_port = htons(PORT);

  if(bind(Room_fd, (SA*)&Room_addr, sizeof(Room_addr)) != 0){
    printf("failed binding socket...\n");
    exit(1);
  }

  if(listen(Room_fd, 5) < 0){
    printf("listen failed...\n");
    exit(1);
  }
  printf("server listenning...\n");

  // allow reuse of local addresses for this addr
  // setsockopt(Room_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

  // prepare queue
  char room[MAX];
  int talker_list[MAX_TALKER];
  int listen_list[MAX_TALKER];
  bzero(room, sizeof(room));
  bzero(talker_list, sizeof(talker_list));
  bzero(listen_list, sizeof(listen_list));

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
    activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
    if(activity == -1){
      printf("Select occurred error...\n");
      exit(0);
    }
    if(FD_ISSET(Room_fd, &readfds)){
      int user = accept(Room_fd, (SA*)NULL, NULL);
      printf("Accept new user at fd %d\n", user);
      if(addNewUser(user, talker_list, listen_list) < 0){
        printf("Can not add user to server\n");
        close(user);
      }
    }

    for(int i=0; i<MAX_TALKER; i++){
      if(talker_list[i] != 0 && FD_ISSET(talker_list[i], &readfds)){
        valread = talk(room, sizeof(room), talker_list[i]);
        if(valread <= 0 || valread > (MAX - 2)){
          printf("fd %d close\n", talker_list[i]);
          close(talker_list[i]);
          talker_list[i] = 0;
        }
        else{
          room[++valread]='\0';
          printf("client [%d] talk:\n %s\n", i, room);
          broad_cast(room, strlen(room), listen_list);
        }
      }
    }
  }
  return 0;
}
