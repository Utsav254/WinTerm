#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "output.h"
#include "linebuf.h"
#include "ansi_escape_codes.h"

void paintStatusBar() {
	char spaces[E.screencols];
	memset(spaces , ' ' , E.screencols - 1);
	spaces[E.screencols] = '\0';

	struct linebuf *line = lbAllocate(E.screencols + ESC_SEQ_EXTRA); //extra room for escape sequences...
	lbAppend(line , HIDE_CURS GRAPH_SEQ(2;7) , HIDE_CURS_l + GRAPH_SEQ_l(GRAPH_FAINT;GRAPH_INV) );

	if((int)strlen(E.path) > PATH_PARTITION-1) {
		lbAppend(line , E.path , PATH_PARTITION - 3);
		lbAppend(line , "..." , 3);
	}
	else {
		lbAppend(line , E.path , strlen(E.path));
	}

	//Pad required white space with space:
	lbAppend(line , spaces , (E.screencols/2) - (line->p - line->e));

	//handle labels for size and date modified... in the second half of the screen.

	lbAppend(line , spaces , ((SIZE_PARTITION)/2) - (SIZE_LABEL_sz)/2);
	lbAppend(line , SIZE_LABEL , SIZE_LABEL_sz);
	lbAppend(line , spaces , ((SIZE_PARTITION)/2) - (SIZE_LABEL_sz)/2);

	//alligning the date and size labels with appropriate padding...

	lbAppend(line , spaces , ((DATE_PARTITION/2)) - (DATE_LABEL_sz)/2);
	lbAppend(line , DATE_LABEL , DATE_LABEL_sz);
	lbAppend(line , spaces , ((DATE_PARTITION/2)) - (DATE_LABEL_sz)/2);

	lbAppend(line , MV_CURS(0,1) SHOW_CURS , MV_CURS_l(0,1) + SHOW_CURS_l);
	writeOut(line->b , E.screencols + line->e);

	lbFree(line);
}

void paintScreen() {
    for (int j = 0; j < E.screenrows; j++) {
        if (j == 0) {
			paintStatusBar();
        }
    }
}

