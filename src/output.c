#include <string.h>
#include <unistd.h>

#include "output.h"
#include "linebuf.h"
#include "ansi_escape_codes.h"
#include "dirio.h"
#include "expl.h"
#include "terminal.h"
#include "numberformat.h"

int size_parition() {
    if (E.screenmode == MINI_MODE) return E.screencols/2;
    else return E.screencols/4;
}

void paintStatusBar(struct linebuf *line) {
    writeOut(MV_CURS_HOME , MV_CURS_HOME_l);
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

    if(E.screenmode != MINI_MODE) {
        lbAppend(line , spaces , ((DATE_PARTITION/2)) - (DATE_LABEL_sz)/2);
        lbAppend(line , DATE_LABEL , DATE_LABEL_sz);
        lbAppend(line , spaces , ((DATE_PARTITION/2)) - (DATE_LABEL_sz)/2);
    }

    int sp = size_parition();
	lbAppend(line , spaces , (sp/2) - (SIZE_LABEL_sz)/2);
	lbAppend(line , SIZE_LABEL , SIZE_LABEL_sz);
	lbAppend(line , spaces , (sp/2) - (SIZE_LABEL_sz)/2);

	lbAppend(line , GRAPH_SEQ(0) MV_CURS(2,1) SHOW_CURS , GRAPH_SEQ_l(0) + MV_CURS_l(2,1) + SHOW_CURS_l);
	writeOut(line->b , line->s);
}

void paintdirents(entries *e , struct linebuf *line , int i) {
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

    if(E.screenmode != MINI_MODE) {
        if(E.screenmode == FULL_MODE) {
            lbAppend(line , spaces , ((DATE_PARTITION)/2) - (16)/2);
            char datebuf[16 + 1];
            memset(datebuf , ' ' , 17);
            ttoa(e->array[i].time , datebuf , 17 , E.screenmode);
            lbAppend(line , datebuf , 16);
            lbAppend(line , spaces , ((DATE_PARTITION/2)) - (16)/2);
        }
        else {
            lbAppend(line , spaces , ((DATE_PARTITION)/2) - 14/2);
            char datebuf[14];
            memset(datebuf , ' ' , 14);
            ttoa(e->array[i].time , datebuf , 14 , E.screenmode);
            lbAppend(line , datebuf , 14);
            lbAppend(line , spaces , ((DATE_PARTITION/2)) - 14/2);
        }
    }
    
    int sp = size_parition();
	lbAppend(line , spaces , (sp/2) - (SIZE_LABEL_sz)/2);
	char sizebuf[SIZE_LABEL_sz + 1];
    memset(sizebuf , ' ' , SIZE_LABEL_sz);
	humanReadableSize(e->array[i].size , sizebuf, 5);
	lbAppend(line , sizebuf , 10);
	lbAppend(line , spaces , (sp/2) - (SIZE_LABEL_sz)/2);


	lbAppend(line , MV_CURS_NEXT_LINE , MV_CURS_NEXT_LINE_l);
	writeOut(line->b , line->s);
}


void paintScreen(void) {
	//for future fixing: its really dumb to allocate withing function which is called in an infinite while loop make this a global var...
    entries *ent = enInit();
    scandirectory(ent , E.path);
//    quickSort(ent , 0 , ent->len-1);
    
    paintStatusBar(lines[0]);

    for (int j = 1; j < E.screenrows; j++) {
        if(j == E.cy || j == E.cy-1) {
            writeOut(GRAPH_SEQ(4) , GRAPH_SEQ_l(4)); // this should be the cursos position

            paintdirents(ent , lines[j] , j-1);

            writeOut(GRAPH_SEQ(0) , GRAPH_SEQ_l(0)); //reset
        } else paintdirents(ent , lines[j] , j-1);
        lbReset(lines[j]);
    }

	enFree(ent);
}

