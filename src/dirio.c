#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "terminal.h"

int getCurrentDir(void) {

    E.path = getcwd(NULL , 0);

    if(E.path == NULL) {
        return 1;
    }
    else {
        return 0;
    }
}
