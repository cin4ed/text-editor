#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

// restore modified flags 
void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCIFLUSH, &orig_termios);
}

// modify terminal flags to stop diplaying characters into the terminal
// - disable canonical mode
// - turn off Ctrl-"x" signals that interrupt with the program
// - at the end call disable rawMode
void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);

  struct termios raw = orig_termios;
  raw.c_iflag &= ~(IXON); 
  raw.c_lflag &= ~(ECHO | ICANON | ISIG); 

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();

  char c;
  // listen for keypresses and display them
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
    if (iscntrl(c)) {
      printf("%d\n", c);
    } else {
      printf("%d ('%c')\n", c, c);
    }
  }

  return 0;
}
