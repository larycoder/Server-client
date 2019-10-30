# include "header.h"

int talk(char* room, int word_sz, int fd){
  bzero(room, word_sz);
  return read(fd, room, word_sz);
}

void hear(char* room, int word_sz, int fd){
  write(fd,room,word_sz);
}

void broad_cast(char* room, int word_sz, int* talker_array){
  for(int i=0;i < MAX_TALKER; i++){
    if(talker_array[i]!=0)
      hear(room, word_sz, talker_array[i]);
  }
}