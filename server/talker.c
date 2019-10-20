# include "header.h"

void talk(char* room, int word_sz, int fd){
  bzero(room,word_sz);
  read(fd,room,word_sz);
}

void hear(char* room, int word_sz, int fd){
  write(fd,room,word_sz);
}