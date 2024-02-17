#ifndef EXPL_H
#define EXPL_H

#include "linebuf.h"
#include "dirio.h"

#define MIN_COL_COUNT 40

//lines structure for the terminal window:
extern struct linebuf **lines;

//entries strucutre to hold directory information
extern entries *ent;

extern int flag;

//--0000
//--4321
//bit1 : bool scandir? -- results in auto refresh of statusbar
//bit2 : bool re-sort? 
//bit3 : bool command bar refresh
//bit4 : bool err message disp?

//cleanup method before exiting program
void cleanUp();

//kill program if error with debugging statement
void die (const char *s);

#endif
