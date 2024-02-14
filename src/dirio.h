#ifndef DIRIO_H
#define DIRIO_H

#include <dirent.h>
#include <sys/stat.h>

#define FILENAME_ASCEND 0
#define FILENAME_DESCEND 1
#define FILESIZE_ASCEND 2
#define FILESIZE_DESCEND 3
#define FILEDATE_ASCEND 4
#define FILEDATE_DESCEND 5

#define UNKNOWN_TYPE 0
#define REGULAR_TYPE 1
#define DIRECTORY_TYPE 2
#define SYMBOLIC_TYPE 3

// Function to get the current working directory
int getCurrentDir(void);

//navigate to some directory
void changeDir(const char *path);

// Structure to represent directory entries
struct entry {
    long size; // in bytes...
    int time;
    int type;
	size_t filename_s;
    char *filename;
};

// Structure to manage a dynamic array of directory entries
typedef struct {
    struct entry *array;
    size_t len;
    size_t capacity;
} entries;

// Function to initialize the entries structure
entries *enInit(void);

// Function to free memory allocated for the entries structure
void enFree(entries *e);

// Function to resize the entries array when it's full
void enResize(entries *e);

// Function to append a new directory entry to the entries array
void enAppend(entries *e, struct dirent *dent, long s, int t , mode_t mode);

// Function to scan a directory and populate the entries structure
void scandirectory(entries *e, const char *path);

//file type:
int file_type(mode_t mode);

//sorting functions:
void enSwap(struct entry *a , struct entry *b);

size_t partition(entries *e, size_t low, size_t high);

void quickSort(entries *e, size_t low, size_t high);

#endif // DIRIO_H
