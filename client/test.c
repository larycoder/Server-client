# include "header.h"

sem_t mutex;
char buff[100];
int cursor = 0;
int oldCursor = 0;

void* tracking(void* arg){
  while(1){
    sem_wait(&mutex);
    cursor = trackUserMess(buff, 99, cursor);
    sem_post(&mutex);
  }
}


void* showMess(void* arg){
  while(1){
    sem_wait(&mutex);
    if(cursor != oldCursor){
      updateUserMess(buff);
      oldCursor = cursor;
    }
    sem_post(&mutex);
  }
}

int main(){
  // setup buff
  bzero(&buff, 100);
  buff[0] = '\0';

  // setup I/O
  if(!(setNonBlockingReading(STDIN_FILENO) == 0)){
    exit(1);
  }

  // setup UI
  enableRawMode();
  drawUI();

  // setup threads
  sem_init(&mutex, 0, 1);
  pthread_t threadTrack, threadShow;
  pthread_create(&threadTrack, NULL, tracking, NULL);
  pthread_create(&threadShow, NULL, showMess, NULL);

  // join thread with main thread
  pthread_join(threadTrack, NULL);
  printf("End thread tracking\n");
  pthread_join(threadShow, NULL);
  printf("End thread show\n");
  sem_destroy(&mutex);
  return 0;
}