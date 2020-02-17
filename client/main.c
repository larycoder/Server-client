# include "header.h"

sem_t mutex;
char buff[MAX];
int readFd = 0, writeFd = 0;

void* trackUserType(void* arg){
  // var used to check user type
  int cursor = 0;
  int enterFlag = 0;
  while(1){
    cursor = trackUserMess(buff, MAX - 1, cursor, &enterFlag); // wait until key press appeared
    sem_wait(&mutex);
    updateUserMess(buff);
    sem_post(&mutex);
    if(enterFlag){
      if(write(writeFd, buff, strlen(buff) - 1) < 0){
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
  char messCome[MAX];
  int length = 0, activity, i = 0;
  fd_set readfds;
  while(1){
    FD_ZERO(&readfds);
    FD_SET(readFd, &readfds);
    activity = select(readFd + 1, &readfds, NULL, NULL, NULL); // wait until has comming mess
    if((length = read(readFd, messCome, sizeof(buff) - 1)) < 0){
      printf("Fail to listen from server \n");
      exit(1);
    }
    else if(length > 0){
      messCome[length] = '\0';
      sem_wait(&mutex);
      updateChatRoom(messCome);
      updateUserMess(buff);
      sem_post(&mutex);
    }
  }
}

void closeSocket(){
  close(readFd);
  close(writeFd);
  sem_destroy(&mutex);
  printf("\033[2J"); // clear terminal
  printf("\033[0;0H"); // jump to begin of terminal
  exit(0);
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
  // if(!(setNonBlockingReading(STDIN_FILENO) == 0)){
  //   exit(1);
  // }

  // setup buff
  bzero(&buff, MAX);
  buff[0] = '\0';

  // setup UI
  enableRawMode();
  drawUI();

  // called when program terminate
  signal(SIGINT, closeSocket);

  // setup threads
  sem_init(&mutex, 0, 1);
  pthread_t trackingUserTypeThread, broadcastMessThread;
  pthread_create(&trackingUserTypeThread, NULL, trackUserType, NULL);
  pthread_create(&broadcastMessThread, NULL, broadcastMess, NULL);

  // join thread with main thread
  pthread_join(trackingUserTypeThread, NULL);
  pthread_join(broadcastMessThread, NULL);
}
