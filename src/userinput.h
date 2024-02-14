#ifndef USERINPUT_H
#define USERINPUT_H

//control key pre processor
#define CTRL_KEY(k) ((k) & 0x1f)

enum exploreKey {
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    BACKSPACE,
};

//read key one by one from user...
int exploreReadKey();

//process keypress using switch case
void processKeypress();

#endif
