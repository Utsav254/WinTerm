#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "ansi_escape_codes.h"
#include "terminal.h"
#include "expl.h"


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

char readKey() {
	int nread;
	char c;
	while((nread = read(STDIN_FILENO , &c , 1)) != 1) {
		if(nread == -1 && errno != EAGAIN) die("read");
	}
	return c;
}