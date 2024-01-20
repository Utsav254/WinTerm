#include <stdio.h>
#include <unistd.h>
#include "terminal.h"
#include "userinput.h"
#include "ansi_escape_codes.h"
#include "output.h"
#include "dirio.h"

struct exploreConfig E;

int main() {

	enableRawMode();

	writeOut(INIT_ALT_TERM_BUF MV_CURS_HOME , INIT_ALT_TERM_BUF_l + MV_CURS_HOME_l);

	getWindowSize(&E.screenrows , &E.screencols);

	if(getCurrentDir() == -1) die("getCurrentDir");
	if(getWindowSize(&E.screenrows , &E.screencols) == -1) die("WindowSize");

	paintScreen();

	while(1) {
		processKeypress();
	}

	return 0;
}