#include <asm-generic/errno-base.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "src/userinput.h"

int exploreReadKey() {
    int nread;
    char c;
    while((nread = read(STDIN_FILENO , &c , 1)) != 1) {
        if(nread == -1 && errno != EAGAIN) exit(1);
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
    else if(c == '\x7f') {
        printf("backspace detect");
        exit(1);
        return 1;
    }
    else {
        return c;
    }
}


int main() {
    while(1){
        int c = exploreReadKey();
        printf("keypress: %d\n" , c);

        int n = c - '1';
        printf("sortmode if therefore : %d\n" , n);
    }
}

