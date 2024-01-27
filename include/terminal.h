#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>

struct exploreConfig {
	struct termios orig_termios;
	int screenrows;
	int screencols;
	int numrows;
	char *path;
};

//globale Explorer window config structure
extern struct exploreConfig E;

//method to write to STDOUT
void writeOut (const char *s , int len);

//disbale terminal raw mode and restore previous terminal settings
void disableRawMode();

//enable raw mode and turn off certain flags
void enableRawMode();

//finds the windows size at the and returns -1 if failed
int getWindowSize(int *rows, int *cols);

//read key from terminal one key at a time
char readKey();

#endif
