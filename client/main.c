# include "header.h"

char buff[MAX];
int readFd = 0, writeFd = 0;

void* trackUserType(void* arg){
  // var used to check user type
  int cursor = 0;
  int oldCursor = 0;
  while(1){
    cursor = trackUserMess(buff, MAX - 1, cursor);
    if(cursor != oldCursor){
      updateUserMess(buff);
      oldCursor = cursor;
    }
  }
}

void closeSocket(void){
  close(readFd);
  close(writeFd);
}

int main(){
  struct sockaddr_in readAddr, writeAddr;
  // socket fd create
  readFd = createFd();
  if(readFd == -1) exit(1);
  writeFd = createFd();
  if(writeFd == -1) exit(1);

  // get ip from user
  char ip[30];
  getIP(ip, sizeof(ip));

  // assign IP, PORT
  bzero(&readAddr, sizeof(readAddr));
  bzero(&writeAddr, sizeof(writeAddr));
  writeAddr.sin_family = readAddr.sin_family = AF_INET;
  writeAddr.sin_addr.s_addr = readAddr.sin_addr.s_addr = inet_addr(ip);
  readAddr.sin_port = htons(R_PORT);
  writeAddr.sin_port = htons(W_PORT);

  // connect the client socket to server socket
  int readConn = connect(readFd, (SA*)&readAddr, sizeof(readAddr));
  int writeConn = connect(writeFd, (SA*)&writeAddr, sizeof(writeAddr));
  if(readConn != 0 || writeConn != 0){
    printf("connection with the server failed...\n");
    exit(1);
  }
  else 
    printf("Connected to the Server..\n");

  // do something !

  // setup I/O
  if(!(setNonBlockingReading(STDIN_FILENO) == 0)){
    exit(1);
  }

  // setup buff
  bzero(&buff, MAX);
  buff[0] = '\0';

  // setup UI
  enableRawMode();
  drawUI();

  // setup threads
  pthread_t trackingUserTypeThread;
  pthread_create(&trackingUserTypeThread, NULL, trackUserType, NULL);

  // join thread with main thread
  pthread_join(trackingUserTypeThread, NULL);

  // close the socket
  atexit(closeSocket);

  return 0;
}
