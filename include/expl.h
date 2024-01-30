#ifndef EXPL_H
#define EXPL_H

#define MIN_COL_COUNT 40

//lines structure for the terminal window:
extern struct linebuf **lines;

//cleanup method before exiting program
void cleanUp();

//kill program if error with debugging statement
void die (const char *s);

#endif
