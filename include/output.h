#ifndef OUTPUT_H
#define OUTPUT_H

#include "terminal.h"

#define PATH_PARTITION E.screencols/2

#define ESC_SEQ_EXTRA 128

#define SIZE_PARTITION E.screencols/4
#define SIZE_LABEL "size"
#define SIZE_LABEL_sz (sizeof(SIZE_LABEL)-1)

#define DATE_PARTITION E.screencols/4
#define DATE_LABEL "date"
#define DATE_LABEL_sz (sizeof(DATE_LABEL)-1)


void paintStatusBar();

void paintScreen();

#endif