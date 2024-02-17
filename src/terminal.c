#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>

#include "terminal.h"
#include "expl.h"
#include "userinput.h"


void writeOut (const char *s , int len) {
    write(STDOUT_FILENO , s , len);
}

void disableRawMode() {
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1) die("tcsetattr");
}

void enableRawMode() {
	if(tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) die("tcgetattr");

	struct termios raw = E.orig_termios;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

int getWindowSize(int *rows, int *cols) {
	struct winsize ws;
	if(ioctl(STDOUT_FILENO , TIOCGWINSZ , &ws) == -1 || ws.ws_col == 0) {
		return -1;
	}
	else {
		*rows = ws.ws_row;
		*cols = ws.ws_col;
		return 0;
	}
}

void exploreMoveCursor(int key) {
    switch(key) {
        case ARROW_LEFT:
            if(E.cx != 0) E.cx--;
            break;
        case ARROW_RIGHT:
            if(E.cx != E.screencols-1) E.cx++;
            break;
        case ARROW_UP:
            if(E.cy != 1) E.cy--;
            break;
        case ARROW_DOWN:
            if((E.cy != (int)(ent->len)) && (E.cy != E.screenrows-1)) E.cy++;
            break;
    }
    fprintf(stderr , "curpos : %d pointing to %s\n" , E.cy-1 , ent->array[E.cy-1].filename);
}





