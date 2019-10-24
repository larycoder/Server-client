# include "header.h"

void talk(char* room, int word_sz, int fd){
  bzero(room,word_sz);
  read(fd,room,word_sz);
}

void hear(char* room, int word_sz, int fd){
  write(fd,room,word_sz);
}

void broad_cast(char* room, int word_sz, int* talker_array, int num_talker){
  printf("client talk: %s\n", room);
  for(int i=0;i < num_talker; i++){
    hear(room, word_sz, talker_array[i]);
  }
}