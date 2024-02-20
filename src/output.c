#include <string.h>
#include <unistd.h>
#include <errno.h>

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
    lbReset(line);
    writeOut(MV_CURS_HOME , MV_CURS_HOME_l);

	lbAppend(line , GRAPH_SEQ(7) , GRAPH_SEQ_l(7) );

    int n = (int)strlen(E.path);

	if(n > PATH_PARTITION-1) {
		lbAppend(line , E.path , PATH_PARTITION - 3);
		lbAppend(line , "..." , 3);
	}
	else lbAppend(line , E.path , n);

	//Pad required white space with space:
    lbListAppend(line , ' ' , PATH_PARTITION - (line->p - line->e));

    if(E.screenmode != MINI_MODE) {
        lbListAppend(line , ' ' , ((DATE_PARTITION/2)) - (DATE_LABEL_sz)/2);
        lbAppend(line , DATE_LABEL , DATE_LABEL_sz);
        lbListAppend(line , ' ' , ((DATE_PARTITION/2)) - (DATE_LABEL_sz)/2);
    }

    int sp = size_parition();
	lbListAppend(line , ' ' , (sp/2) - (SIZE_LABEL_sz)/2);
	lbAppend(line , SIZE_LABEL , SIZE_LABEL_sz);
	lbListAppend(line , ' ' , (sp/2) - (SIZE_LABEL_sz)/2);

	lbAppend(line , GRAPH_SEQ(0) MV_CURS(2,1), GRAPH_SEQ_l(0) + MV_CURS_l(2,1));
	writeOut(line->b , line->s);
}

void paintdirents(entries *e , struct linebuf *line , int i) {
    lbReset(line);
    lbAppend(line , CLR_CURR_LINE , CLR_CURR_LINE_l);
    //check for out of range index
	if(i >= (int)e->len) return;

	if((int)e->array[i].filename_s > PATH_PARTITION-1) {
		lbAppend(line , e->array[i].filename , PATH_PARTITION - 3);
		lbAppend(line , "..." , 3);
	}
	else {
		lbAppend(line , e->array[i].filename  , (int)e->array[i].filename_s);
	}

	lbListAppend(line , ' ' , PATH_PARTITION - (line->p - line->e));

    if(E.screenmode != MINI_MODE) {
        if(E.screenmode == FULL_MODE) {
            lbListAppend(line , ' ' , ((DATE_PARTITION)/2) - (16)/2);
            char datebuf[FULL_DATE_sz];
            ttoa(e->array[i].time , datebuf , FULL_DATE_sz , E.screenmode);
            lbAppend(line , datebuf , FULL_DATE_sz);
            lbListAppend(line , ' ' , ((DATE_PARTITION/2)) - (16)/2);
        }
        else {
            lbListAppend(line , ' ' , ((DATE_PARTITION)/2) - 14/2);
            char datebuf[MEDIUM_DATE_sz];
            ttoa(e->array[i].time , datebuf , MEDIUM_DATE_sz, E.screenmode);
            lbAppend(line , datebuf , MEDIUM_DATE_sz);
            lbListAppend(line , ' ' , ((DATE_PARTITION/2)) - 14/2);
        }
    }
    
    int sp = size_parition();
	lbListAppend(line , ' ' , (sp/2) - (SIZE_LABEL_sz)/2);
    //check if it is a directory
    if(e->array[i].type == DIRECTORY_TYPE) {
        lbAppend(line , " -- " , 4);
    }
    //append file size in human readable form
    else {
        char sizebuf[SIZE_LABEL_sz + 1];
        memset(sizebuf , ' ' , SIZE_LABEL_sz);
        humanReadableSize(e->array[i].size , sizebuf, 5);
        lbAppend(line , sizebuf , SIZE_LABEL_sz);
    }
    lbListAppend(line , ' ' , (sp/2) - (SIZE_LABEL_sz)/2);
	lbAppend(line , MV_CURS_NEXT_LINE , MV_CURS_NEXT_LINE_l);
	writeOut(line->b , line->s);
}

void paintCommandBar(struct linebuf *line) {
    lbReset(line);
    //move cursor to buttom...
    writeOut(MV_CURS(999 , 1) , MV_CURS_l(999 , 1));

    //check if errnum set ... TODO: imrpove errmsg formatting
    if(flag & 8) {
        lbAppend(line , GRAPH_SEQ(31) , GRAPH_SEQ_l(31));
        char *errmsg = strerror(errno);
        int n = (int)strlen(errmsg);

        if(n > PATH_PARTITION-1) {
            lbAppend(line , errmsg , PATH_PARTITION - 3);
            lbAppend(line , "..." , 3);
        }
        else lbAppend(line , errmsg , n);

        lbAppend(line , GRAPH_SEQ(0) , GRAPH_SEQ_l(0));
    }

    //write to console and reset flag
	writeOut(line->b , line->s);
    flag ^= 12;
}


void paintScreen(void) {

    //main printing loop
    for (int j = 0; j < E.screenrows; j++) {
        //check if flag[0] is set for scan directory
        if(j == 0 && (flag & 1) == 1) {
            writeOut(CLR_SCR_BUF , CLR_SCR_BUF_l);

            enFree(ent);
            ent = enInit();
            scandirectory(ent , E.path);

            //reset scroll_offset if required
            if((E.screenrows-2) > (int)ent->len) E.scroll_offset = 0;

            //if cursos is off the page, reset position to lowest poition
            if((E.cy - 1) > (int)(ent->len-1)) E.cy = (int)(ent->len);

            //change in status bar required...
            paintStatusBar(lines[0]);

            //reset scan flag
            flag ^= 1;
        }
        //check if sorting is required
        if((flag & 2) == 2) {
            quickSort(ent , 0 , ent->len-1);
            flag ^= 2;
        }

        if(j > 0 && j < E.screenrows-1) {
            if(j == E.cy || j == E.cy-1) {
                writeOut(GRAPH_SEQ(4) , GRAPH_SEQ_l(4)); // this should be the cursos position
                paintdirents(ent , lines[j] , j-1+E.scroll_offset);
                writeOut(GRAPH_SEQ(0) , GRAPH_SEQ_l(0)); //reset
            }
            else paintdirents(ent , lines[j] , j-1+E.scroll_offset);
        }

        //flag check and print commandBar
        if(j == E.screenrows-1 && (flag & 4) == 4) paintCommandBar(lines[j]);
    }
    writeOut(MV_CURS(2,1) , MV_CURS_l(2,1));
}

