#include "header.h"

int main(){
  // declare queues
  char room[MAX];
  int talker_list[MAX_TALKER];
  int listen_list[MAX_TALKER];

  // setup listenning socket
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

  // setup queues
  bzero(room, sizeof(room));
  bzero(talker_list, sizeof(talker_list));
  bzero(listen_list, sizeof(listen_list));

  // prepare for select
  fd_set talkFds;
  int talkNfds, activity;
  int len;
  // server real running time
  while(1){
    talkNfds = resetQueue(&talkFds, talker_list, MAX_TALKER);
    talkNfds = add2Queue(&talkFds, listen_list, MAX_TALKER, talkNfds);
    // add main socket to talk set
    FD_SET(Room_fd, &talkFds);
    if(Room_fd > (talkNfds - 1)){
      talkNfds = Room_fd + 1;
    }

    // wait for the activity
    activity = select(talkNfds, &talkFds, NULL, NULL, NULL);
    if(activity == -1){
      printf("Select occurred error...\n");
      exit(0);
    }

    if(FD_ISSET(Room_fd, &talkFds)){
      int user = accept(Room_fd, (SA*)NULL, NULL);
      printf("Accept new user at fd %d\n", user);
      if(addNewUser(user, talker_list, listen_list) < 0){
        printf("Can not add user to server\n");
        close(user);
      }
    }

    for(int i = 0; i < MAX_TALKER; i++){
      if(listen_list[i] != 0 && FD_ISSET(listen_list[i], &talkFds)){
        printf("hear_fd %d close\n", listen_list[i]);
        close(listen_list[i]);
        listen_list[i] = 0;
      }
    }

    for(int i = 0; i < MAX_TALKER; i++){
      if(talker_list[i] != 0 && FD_ISSET(talker_list[i], &talkFds)){
        len = talk(room, sizeof(room) - 1, talker_list[i]);
        if(len < 1){
          printf("talk_fd %d close\n", talker_list[i]);
          close(talker_list[i]);
          talker_list[i] = 0;
        }
        else{
          room[len + 1] = '\0';
          printf("client [%d] talk:\n %s\n", i, room);
          broad_cast(room, strlen(room), listen_list);
        }
      }
    }
  }
  return 0;
}
