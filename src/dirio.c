#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "terminal.h"
#include "expl.h"

#include "dirio.h"

int getCurrentDir(void) {

    E.path = getcwd(NULL , 0);

    if(E.path == NULL) return 1;
    else return 0;
}


entries *enInit() {
    entries *e = (entries *)malloc(sizeof(entries));
    if(e == NULL) die("malloc(entries");

    //initial capacity to store a minimum of the . and .. directories
    e->array = (struct entry *)malloc(2 * sizeof(struct entry));
    if(e->array == NULL) die("malloc(struct entry)");
    e->capacity = 2;
    e->len = 0;

    return e;
}

void enFree(entries *e) {
    for(int i = 0 ; i < (int)e->len ; i++) {
        free(e->array[i].filename);
    }
    free(e->array);
    free(e);
}

void enResize(entries *e) {
    e->array = realloc(e->array , (e->capacity*2)*sizeof(struct entry));
    if(e->array == NULL) die("realloc(e->array)");
    e->capacity *= 2;
}

void enAppend(entries *e , struct dirent * dent , int s , int t) {
    if(e->len == e->capacity) {
        enResize(e);
    }

    e->array[e->len].filename = strdup(dent->d_name);
    e->array[e->len].time = t;
    e->array[e->len].size = s;
    e->len++;
}

void scandirectory(entries *e , const char *path) {
    DIR *dir = opendir(path);
    if(dir == NULL) die("opendir");

    struct dirent *dent;
    struct stat fstat;

    while((dent = readdir(dir)) != NULL) {
        stat(dent->d_name , &fstat);
        enAppend(e , dent , fstat.st_size , fstat.st_mtime);
    }
    closedir(dir);
}

