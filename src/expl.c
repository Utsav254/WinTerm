#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "expl.h"
#include "terminal.h"
#include "ansi_escape_codes.h"
#include "linebuf.h"

struct exploreConfig E;

struct linebuf **lines;

void cleanUp(void) {

	for(int i = 0 ; i < E.screenrows ; i++) {
		lbFree(lines[i]);
	}
	free(lines);
	free(E.path);
	writeOut(END_ALT_TERM_BUF , END_ALT_TERM_BUF_l);
	disableRawMode();
}

void die (const char *s) {
	cleanUp();
	write(STDERR_FILENO , s , strlen(s));
	exit(1);
}

int main(void) {
	//init application
//	enableRawMode();
//
//	writeOut(INIT_ALT_TERM_BUF MV_CURS_HOME , INIT_ALT_TERM_BUF_l + MV_CURS_HOME_l);
//	//fetch current working directory
//	if(getCurrentDir() == -1) die("getCurrentDir");
//	//fetch terminal window size
//	if(getWindowSize(&E.screenrows , &E.screencols) == -1) die("WindowSize");
//
//	//allocate linebuffers for each row
//	lines = malloc(E.screenrows * sizeof(struct linebuf*));
//	if(lines == NULL) die("malloc(lines)");
//	for(int i = 0 ; i < E.screenrows ; i++) {
//		lines[i] = lbAllocate(E.screencols + ESC_SEQ_EXTRA);
//	}
//
//	//if terminal is too small exit...
//	if(E.screencols < MIN_COL_COUNT) die("terminal window too small\n");
//
//
//	
//	//program loop:
//	while(1) {
//		paintScreen();
//		processKeypress();
//	}
    

    return 0;
}

