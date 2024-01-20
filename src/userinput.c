#include <stdlib.h>

#include "userinput.h"
#include "terminal.h"

void processKeypress() {
	char c = readKey();
	switch (c) {
		case CTRL_KEY('q'):
			cleanUp();
			exit(0);
			break;
	}
}