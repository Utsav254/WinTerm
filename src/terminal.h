#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>

#define FULL_MODE_COND 100
#define MEDIUM_MODE_COND 60
#define MINI_MODE_COND 30

#define FULL_MODE 0
#define MEDIUM_MODE 1
#define MINI_MODE 2

struct exploreConfig {
	struct termios orig_termios;
	int screenrows;
	int screencols;
    int screenmode;
	char *path;
    int sortmode;
    int scroll_offset;
    int cx , cy;
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

//handler for moving cursor
void exploreMoveCursor(int  key);

#endif
