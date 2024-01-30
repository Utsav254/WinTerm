#include <stdlib.h>

#include "userinput.h"
#include "terminal.h"
#include "expl.h"
#include "ansi_escape_codes.h"

void processKeypress() {
	char c = readKey();
	switch (c) {
		case CTRL_KEY('q'):
			cleanUp();
			exit(0);
			break;
		default:
			writeOut(&c , 1);
			writeOut(MV_CURS_HOME , MV_CURS_HOME_l);
			break;
	}
}