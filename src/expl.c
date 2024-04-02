#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "expl.h"
#include "output.h"
#include "dirio.h"
#include "userinput.h"
#include "terminal.h"
#include "ansi_escape_codes.h"
#include "linebuf.h"

struct exploreConfig E;

struct linebuf **lines;

entries *ent;

int flag = 64;

void cleanUp(void) {

	for(int i = 0 ; i < E.screenrows ; i++) {
		lbFree(lines[i]);
	}
	free(lines);

    if(ent != NULL) enFree(ent);
	writeOut(END_ALT_TERM_BUF SHOW_CURS, END_ALT_TERM_BUF_l + SHOW_CURS_l);
	disableRawMode();

    free(E.path);
}

void die (const char *s) {
	cleanUp();
	write(STDERR_FILENO , s , strlen(s));
	exit(1);
}

int main(void) {
	//init application
	enableRawMode();
	writeOut(INIT_ALT_TERM_BUF MV_CURS_HOME HIDE_CURS, INIT_ALT_TERM_BUF_l + MV_CURS_HOME_l + HIDE_CURS_l);

	//fetch current working directory
	if(getCurrentDir() == -1) die("getCurrentDir");

	//fetch terminal window size
	if(getWindowSize(&E.screenrows , &E.screencols) == -1) die("WindowSize\n");
    //decide window size mode:
    if(E.screencols > FULL_MODE_COND) E.screenmode = FULL_MODE;
    else if(E.screencols > MEDIUM_MODE_COND) E.screenmode = MEDIUM_MODE;
    else if(E.screencols > MINI_MODE_COND) E.screenmode = MINI_MODE;
    else die("Window Size is too small\n");

    E.cx = 0;
    E.cy = 1;
    E.scroll_offset = 0;
    
    //initialise directory data:
    E.sortmode = FILENAME_ASCEND;
    ent = enInit();
    scandirectory(ent , E.path);
    quickSort(ent , 0 , ent->len-1);
    //set flag to force printing
    flag ^= 3; 

	//allocate linebuffers for each row
	lines = malloc(E.screenrows * sizeof(struct linebuf*));
	if(lines == NULL) die("malloc(lines)");
	for(int i = 0 ; i < E.screenrows ; i++) {
		lines[i] = lbAllocate(E.screencols + ESC_SEQ_EXTRA);
	}

    paintStatusBar(lines[0]);
	
    //program loop:
	while(1) {
		paintScreen();
		processKeypress();
	}
    return 0;
}

