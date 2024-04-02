#include "expl.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <stdio.h>

//control key pre processor
#define CTRL_KEY(k) ((k) & 0x1f)

enum exploreKey {
    BACKSPACE = 127,
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    PAGE_UP,
    PAGE_DOWN,
    HOME_KEY,
    END_KEY,
    DEL_KEY,
    MOUSE_in,
    MOUSE_out,
};

struct exploreConfig {
	struct termios orig_termios;
	int screenrows;
	int screencols;
    int screenmode;
	char *path;
    int sortmode;
    int scroll_offset;
    int cx , cy;
    int mx , my;
} E;


void disableRawMode() {
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1) exit(1);
    write(STDOUT_FILENO , "\x1b[?1000l" , 8);
}

void enableRawMode() {
    atexit(cleanUp);
	if(tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) exit(1);

    write(STDOUT_FILENO , "\x1b[?1000h" , 8);

	struct termios raw = E.orig_termios;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) exit(1);
}

int exploreReadKey() {
    int nread;
    char c;
    while((nread = read(STDIN_FILENO , &c , 1)) != 1) {
        if(nread == -1 && errno != EAGAIN) exit(1);
    }
    if(c == '\x1b') {
        char seq[10];

        if(read(STDIN_FILENO , &seq[0] , 1) != 1) return '\x1b';
        if(read(STDIN_FILENO , &seq[1] , 1) != 1) return '\x1b';

        if(seq[0] == '[') {
            if(seq[1] >= '0' && seq[1] <= '9') {
                if(read(STDIN_FILENO , &seq[2] , 1) != 1) return '\x1b';
                if(seq[2] == '~') {
                    switch(seq[1]){
                        case '1': return HOME_KEY;
                        case '3': return DEL_KEY;
                        case '4': return END_KEY;
                        case '5': return PAGE_UP;
                        case '6': return PAGE_DOWN;
                        case '7': return HOME_KEY;
                        case '8': return END_KEY;

                    }
                }
            }
            else {
                switch (seq[1]) {
                    case 'A' : return ARROW_UP;
                    case 'B' : return ARROW_DOWN;
                    case 'C' : return ARROW_RIGHT;
                    case 'D' : return ARROW_LEFT;
                    case 'H' : return HOME_KEY;
                    case 'F' : return END_KEY;
                    case 'M' : 
                        if(read(STDIN_FILENO , &seq[2] , 1) != 1) return '\x1b';
                        if(read(STDIN_FILENO , &seq[3] , 1) != 1) return '\x1b';
                        if(read(STDIN_FILENO , &seq[4] , 1) != 1) return '\x1b';
                        switch(seq[2]){
                            case 32:
                                printf("mouse in\n\r");
                                return MOUSE_in;
                                break;
                            case 35:
                                return MOUSE_out;
                                break;
                        }  
                }
            }
        }
        else if(seq[0] == 'O') {
            switch(seq[1]) {
                case 'H': return HOME_KEY;
                case 'F': return END_KEY;
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
            exit(1);
            break;

        case ARROW_UP: 
            printf("arrow up\n\r");
            break;
        case ARROW_DOWN: 
            printf("arrow down\n\r");
            break;
        case ARROW_LEFT: 
            printf("arrow left\n\r");
            break;
        case ARROW_RIGHT: 
            printf("arrow right\n\r");
            break;
        case PAGE_UP:
            printf("page up\n\r");
            break;
        case PAGE_DOWN:
            printf("page down\n\r");
            break;
        case HOME_KEY:
            printf("home key\n\r");
            break;
        case END_KEY:
            printf("end key\n\r");
            break;
        case DEL_KEY:
            printf("delete key\n\r");
            break;


        case '\r':
            printf("carriage ret\n\r");
            break;
        case BACKSPACE:
            printf("back space\n\r");
            break;
        
        default:
            if(iscntrl(c)) printf("control: %d\n\r" , c);
            else printf("%d ('%c')\r\n", c, c);
            break;
    }
}

int main() {
    E.mx = 0;
    E.my = 0;
    enableRawMode();

    while(1){
        processKeypress();
    }

    disableRawMode();
    return 0;
}
