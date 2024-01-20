#ifndef USERINPUT_H
#define USERINPUT_H

//control key pre processor
#define CTRL_KEY(k) ((k) & 0x1f)

//process keypress using switch case
void processKeypress();

#endif