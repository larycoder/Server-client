# include "header.h"

struct termios orig_termios;

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode); // terminal back to normal state when exit
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);
  // raw.c_oflag &= ~(OPOST); // turn off output processing
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void drawUI(void){
  printf("\033[2J"); // clear terminal
  updateUserMess("");
}

void updateUserMess(char* messEnter){
  printf("\033[0;0H\033[K"); // move to enter field
  printf("Text Enter: %s", messEnter);
  fflush(stdout);
}

void updateChatRoom(char* newMessCome){
  printf("\033[u"); // store cursor position
  printf("\n"); // begin new chat line
  printf("%s", newMessCome);
  fflush(stdout);
  printf("\033[u"); // restore cursor to enter field
}