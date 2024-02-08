#include <string.h>
#include <unistd.h>

#include "output.h"
#include "linebuf.h"
#include "ansi_escape_codes.h"
#include "dirio.h"
#include "expl.h"
#include "numberformat.h"

void paintStatusBar(struct linebuf *line) {
	lbReset(line);
	char spaces[E.screencols];
	memset(spaces , ' ' , E.screencols - 1);
	spaces[E.screencols] = '\0';

	lbAppend(line , GRAPH_SEQ(7) , GRAPH_SEQ_l(7) );

	if((int)strlen(E.path) > PATH_PARTITION-1) {
		lbAppend(line , E.path , PATH_PARTITION - 3);
		lbAppend(line , "..." , 3);
	}
	else {
		lbAppend(line , E.path , strlen(E.path));
	}

	//Pad required white space with space:
	lbAppend(line , spaces , PATH_PARTITION - (line->p - line->e));

	//handle labels for size and date modified... in the second half of the screen.

	lbAppend(line , spaces , ((SIZE_PARTITION)/2) - (SIZE_LABEL_sz)/2);
	lbAppend(line , SIZE_LABEL , SIZE_LABEL_sz);
	lbAppend(line , spaces , ((SIZE_PARTITION)/2) - (SIZE_LABEL_sz)/2);

	//alligning the date and size labels with appropriate padding...

	lbAppend(line , spaces , ((DATE_PARTITION/2)) - (DATE_LABEL_sz)/2);
	lbAppend(line , DATE_LABEL , DATE_LABEL_sz);
	lbAppend(line , spaces , ((DATE_PARTITION/2)) - (DATE_LABEL_sz)/2);

	lbAppend(line , GRAPH_SEQ(0) MV_CURS(2,1) SHOW_CURS , GRAPH_SEQ_l(0) + MV_CURS_l(2,1) + SHOW_CURS_l);
	writeOut(line->b , line->s);

}

void paintdirents(entries *e , struct linebuf *line , int i) {
	lbReset(line);
	if(i >= (int)e->len) return;
    
	char spaces[E.screencols];
	memset(spaces , ' ' , E.screencols - 1);
	spaces[E.screencols-1] = '\0';

	if((int)e->array[i].filename_s > PATH_PARTITION-1) {
		lbAppend(line , e->array[i].filename , PATH_PARTITION - 3);
		lbAppend(line , "..." , 3);
	}
	else {
		lbAppend(line , e->array[i].filename  , (int)e->array[i].filename_s);
	}

	lbAppend(line , spaces , PATH_PARTITION - (line->p - line->e));

	lbAppend(line , spaces , ((SIZE_PARTITION)/2) - (SIZE_LABEL_sz)/2);
	char sizebuf[5];
    memset(sizebuf , ' ' , 5);
	humanReadableSize(e->array[i].size , sizebuf, 5);
	lbAppend(line , sizebuf , 10);
	lbAppend(line , spaces , ((SIZE_PARTITION)/2) - (SIZE_LABEL_sz)/2);


    char datebuf[20];
    memset(datebuf , ' ' , 20);
    itoa(e->array[i].time , datebuf , 20);
    lbAppend(line , datebuf , 10);
	lbAppend(line , spaces , ((DATE_PARTITION/2)) - (DATE_LABEL_sz)/2);


	lbAppend(line , MV_CURS_NEXT_LINE , MV_CURS_NEXT_LINE_l);
	writeOut(line->b , line->s);
}


void paintScreen() {
	//for future fixing: its really dumb to allocate withing function which is called in an infinite while loop make this a global var...
	entries *ent = enInit();
	scandirectory(ent , E.path);

    for (int j = 0; j < E.screenrows; j++) {
        if (j == 0) {
			paintStatusBar(lines[0]);
        }
		else {
			if(j == 1) {
				writeOut(GRAPH_SEQ(4) , GRAPH_SEQ_l(4)); // this should be the cursos position

				paintdirents(ent , lines[j] , j-1);

				writeOut(GRAPH_SEQ(0) , GRAPH_SEQ_l(0)); //reset
			}
			else paintdirents(ent , lines[j] , j-1);
		}
    }

	enFree(ent);
}

