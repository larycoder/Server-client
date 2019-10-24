#include "header.h"

int talker_list[MAX_TALKER];
char room[MAX];
fd_set readfds;
int flag=0, max_fd=0, talker_index=0;
sem_t mutex;

void* staff_thread(void* lfd);
void* talker_thread(void* arg);

int main(){
  bzero(talker_list, MAX_TALKER);
  bzero(room, MAX);

  // prepare listening socket
  struct sockaddr_in Room_addr;
  int Room_fd = open_chat_room(Room_addr);

  // prepare semaphores
  sem_init(&mutex, 0, 1);

  // thread of staff
  pthread_t staff_id;
  pthread_create(&staff_id, NULL, staff_thread, &Room_fd);

  // thread of talker
  pthread_t talker_id;
  pthread_create(&talker_id, NULL, talker_thread, NULL);

  // wait for all thread join
  pthread_join(talker_id, NULL);
  printf("close talker thread\n");
  pthread_join(staff_id, NULL);
  printf("close staff thread\n");

  sem_destroy(&mutex);

  return 0;
}

void* staff_thread(void* lfd){
  int listen_fd = *((int*)lfd);
  register_talker(listen_fd, &flag, &readfds, &max_fd, talker_list, &talker_index, &mutex);
  return NULL;
}

void* talker_thread(void* arg){
  while(1){
    // this is atomic work
    sem_wait(&mutex);
    int check = select(max_fd+1, &readfds, NULL, NULL, NULL);
    if(check == -1){
      printf("error occurred in select\n");
      flag = CLOSE_FLAG;
      exit(0);
    }
    for(int i=0;i<talker_index;i++){
      if(FD_ISSET(talker_list[i], &readfds)){
        talk(room, MAX, talker_list[i]);
        broad_cast(room, sizeof(room), talker_list, talker_index);
      }
    }
    sem_post(&mutex);
    if(flag == CLOSE_FLAG) return NULL;
  }
}