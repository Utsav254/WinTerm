#ifndef DIRIO_H
#define DIRIO_H

// Function to get the current working directory
int getCurrentDir(void);

// Structure to represent directory entries
struct entry {
    int size; // in bytes...
    int time;
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
void enAppend(entries *e, struct dirent *dent, int s, int t);

// Function to scan a directory and populate the entries structure
void scandirectory(entries *e, const char *path);

#endif // DIRIO_H
