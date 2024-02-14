#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "userinput.h"
#include "terminal.h"
#include "expl.h"
#include "dirio.h"

int exploreReadKey() {
    int nread;
    char c;
    while((nread = read(STDIN_FILENO , &c , 1)) != 1) {
        if(nread == -1 && errno != EAGAIN) die("read key function\n");
    }
    if(c == 'h') return BACKSPACE;
    else if(c == '\x1b') {
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
		case CTRL_KEY('q'):
			cleanUp();
			exit(0);
			break;
        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_RIGHT:
        case ARROW_LEFT:
            exploreMoveCursor(c);
            break;
        case BACKSPACE: changeDir(".."); 
	}
}
