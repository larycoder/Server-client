# include "header.h"

sem_t mutex;
char buff[MAX];
int readFd = 0, writeFd = 0;

void* trackUserType(void* arg){
  // var used to check user type
  int cursor = 0;
  int oldCursor = 0;
  int enterFlag = 0;
  while(1){
    cursor = trackUserMess(buff, MAX - 1, cursor, &enterFlag);
    if(cursor != oldCursor){
      sem_wait(&mutex);
      updateUserMess(buff);
      sem_post(&mutex);
      oldCursor = cursor;
    }
    if(enterFlag){
      if(write(writeFd, buff, strlen(buff)) < 0){
        printf("fail to send mess to server\n");
        exit(1);
      }
      cursor = 0;
      buff[0] = '\0';
      enterFlag = 0;
    }
  }
}

void* broadcastMess(void* arg){
  char buff[MAX];
  int length = 0;
  while(1){
    if((length = read(writeFd, buff, sizeof(buff) - 1)) < 0){
      printf("Fail to listen from server \n");
      exit(1);
    }
    else if(length > 0){
      buff[length + 1] = '\0';
      sem_wait(&mutex);
      updateChatRoom(buff);
      sem_post(&mutex);
    }
  }
}

void closeSocket(void){
  close(readFd);
  close(writeFd);
  sem_destroy(&mutex);
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

  // send first mess to server
  char one = '1', zero = '0';
  int wrMess = write(readFd, &one, sizeof(char));
  int rdMess = write(writeFd, &zero, sizeof(char));
  if(wrMess < 0 || rdMess < 0){
    printf("Fail to confirm type with server\n");
    exit(1);
  }

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

  // close the socket
  atexit(closeSocket);

  // setup threads
  sem_init(&mutex, 0, 1);
  pthread_t trackingUserTypeThread, broadcastMessThread;
  pthread_create(&trackingUserTypeThread, NULL, trackUserType, NULL);
  pthread_create(&broadcastMessThread, NULL, broadcastMess, NULL);

  // join thread with main thread
  pthread_join(trackingUserTypeThread, NULL);
  pthread_join(broadcastMessThread, NULL);

  return 0;
}
