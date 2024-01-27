#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

#include "output.h"
#include "linebuf.h"
#include "ansi_escape_codes.h"
#include "dirio.h"
#include "expl.h"

#define _GNU_SOURCE_

void paintStatusBar(struct linebuf *line) {
	char spaces[E.screencols];
	memset(spaces , ' ' , E.screencols - 1);
	spaces[E.screencols] = '\0';

	//struct linebuf *line = lbAllocate(E.screencols + ESC_SEQ_EXTRA); //extra room for escape sequences...
	lbAppend(line , GRAPH_SEQ(7) , GRAPH_SEQ_l(7) );

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

	lbAppend(line , GRAPH_SEQ(0) MV_CURS(2,1) SHOW_CURS , GRAPH_SEQ_l(0) + MV_CURS_l(2,1) + SHOW_CURS_l);
	writeOut(line->b , E.screencols + line->e);

}

int compare(const void *a, const void *b) {
    return strcmp(((struct dirent *)a)->d_name, ((struct dirent *)b)->d_name);
}

void paintScreen() {
	struct dirent **namelist;
	int n;

	n = scandir(E.path , &namelist , NULL , alphasort);
	if(n == -1) die("scandir\n");


    for (int j = 0; j < E.screenrows; j++) {
        if (j == 0) {
			paintStatusBar(lines[0]);
        }
		else {
			if(j < n){

				char filename [256];//maximum file name size in linux
				strncpy(filename , namelist[j]->d_name , sizeof filename -1);
				filename[255] = '\0';

				if((int)strlen(filename) > PATH_PARTITION-1) {
					lbAppend(lines[j] , filename , PATH_PARTITION - 3);
					lbAppend(lines[j] , "..." , 3);
				}
				else {
					lbAppend(lines[j] , filename , strlen(E.path));
				}
				lbAppend(lines[j] , "\n\r" ,  2);
				writeOut(lines[j]->b , lines[j]->s);
			}
		}
    }
	for (int i = 0; i < n; ++i) {
		free(namelist[i]);
	}
	free(namelist);
}
