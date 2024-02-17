#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "userinput.h"
#include "terminal.h"
#include "expl.h"
#include "dirio.h"

void processSortSelect() {
    int c = exploreReadKey();
    if(c >= '1' && c <= '6') {
        if(E.sortmode != (c - '1')) {
            E.sortmode = c - '1';
            flag |= 2;
        }
    }
    else return;
}

int exploreReadKey() {
    int nread;
    char c;
    while((nread = read(STDIN_FILENO , &c , 1)) != 1) {
        if(nread == -1 && errno != EAGAIN) die("read key function\n");
    }
    if(c == '\x1b') {
        char seq[3];

        if(read(STDIN_FILENO , &seq[0] , 1) != 1) return '\x1b';
        if(read(STDIN_FILENO , &seq[1] , 1) != 1) return '\x1b';
        if(seq[0] == '[') {
            switch (seq[1]) {
                case 'A' : return ARROW_UP;
                case 'B' : return ARROW_DOWN;
                case 'C' : return ARROW_RIGHT;
                case 'D' : return ARROW_LEFT;
            }
        }
        return '\x1b';
    }
    else {
        return c;
    }
}

void processKeypress() {
	int c = exploreReadKey();
	switch (c) {

        //basic control characters
        case '\r':
            fprintf(stderr , "%p\n" , &ent->array[E.cy-1].filename);
            changeDir(ent->array[E.cy-1].filename);
            break;

        //control keymaps
		case CTRL_KEY('q'):
			cleanUp();
			exit(0);
			break;
        case CTRL_KEY('s'):
            processSortSelect();
            break;

        //arrow keymaps
        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_RIGHT:
        case ARROW_LEFT:
            exploreMoveCursor(c);
            break;
        case BACKSPACE:
        //letter keymaps
        case 'h': 
            changeDir("..");
            break;
        case 'j': 
            exploreMoveCursor(ARROW_DOWN);
            break;
        case 'k': 
            exploreMoveCursor(ARROW_UP);
            break;
	}
}
