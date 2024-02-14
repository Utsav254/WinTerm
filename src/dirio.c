#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "terminal.h"
#include "expl.h"
#include "dirio.h"


int getCurrentDir(void) {

    E.path = getcwd(NULL , 0);

    if(E.path == NULL) return 1;
    else return 0;
}

void changeDir(const char *path) {
    if(chdir(path) != 0) die("changeDir");
    if(getCurrentDir() == 1) die("getCurrentDir\n");
}


entries *enInit(void) {
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

void enAppend(entries *e , struct dirent * dent , long s , int t , mode_t mode) {
    if(e->len == e->capacity) {
        enResize(e);
    }

	char *temp = strdup(dent->d_name);

    e->array[e->len].filename = temp;
    e->array[e->len].time = t;
    e->array[e->len].type = file_type(mode); 
    e->array[e->len].size = s;
	e->array[e->len].filename_s = strlen(temp);
    e->len++;
}

int file_type (mode_t mode) {
    switch(mode & S_IFMT) {
        case S_IFREG:
            return REGULAR_TYPE;
            break;
        case S_IFDIR:
            return DIRECTORY_TYPE;
            break;
        case S_IFLNK:
            return SYMBOLIC_TYPE;
            break;
        default:
            return UNKNOWN_TYPE;
            break;
    }
}

void scandirectory(entries *e , const char *path) {
    enFree(e);
    e = enInit();

    DIR *dir = opendir(path);
    if(dir == NULL) die("opendir");

    struct dirent *dent;
    struct stat fstat;

    while((dent = readdir(dir)) != NULL) {
        if(strcmp(dent->d_name , ".") == 0 || strcmp(dent->d_name , "..") == 0) continue;
        else {
             stat(dent->d_name , &fstat);
             enAppend(e , dent , (long)fstat.st_size , fstat.st_mtime , fstat.st_mode);
        }        
    }
    closedir(dir);
}

void enSwap(struct entry *a , struct entry *b) {
    struct entry temp = *a;
    *a = *b;
    *b = temp;
} 

int compare(struct entry *a , struct entry *b) {
    if(strcmp(a->filename ,"..") == 0) return -1;
    else if(strcmp(b->filename , "..") == 0) return 1;
    else if(a->type == DIRECTORY_TYPE && b->type == REGULAR_TYPE) return -1;
    else if(b->type == DIRECTORY_TYPE && a->type == REGULAR_TYPE) return 1;
    else {
        switch(E.sortmode){
        case FILENAME_ASCEND:
            return strcasecmp(a->filename , b->filename);
            break;
        case FILENAME_DESCEND:
            return strcasecmp(b->filename , a->filename);
            break;
        case FILESIZE_ASCEND:
            if(a->size > b->size) return -1;
            else if(b->size > a->size) return 1;
            else return 0;
            break;
        case FILESIZE_DESCEND:
            if(a->size < b->size) return -1;
            else if(b->size < a->size) return 1;
            else return 0;
            break;
        default:
            break;    
        }
        die("invalid sort mode\n");
        return 0;
    }
}

size_t partition(entries *e, size_t low, size_t high) {
    struct entry pivot = e->array[high];
    size_t i = low - 1;

    for (size_t j = low; j < high; j++) {
        if (compare(&e->array[j] ,&pivot) < 0) {
            i++;
            enSwap(&e->array[i], &e->array[j]);
        }
    }

    enSwap(&e->array[i + 1], &e->array[high]);
    return i + 1;
}

void quickSort(entries *e, size_t low, size_t high) {
    if (low < high) {
        size_t pi = partition(e, low, high);

        if (pi > 0) {
            quickSort(e, low, pi - 1);
        }

        quickSort(e, pi + 1, high);
    }
}


