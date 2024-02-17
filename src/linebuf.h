#ifndef LINEBUF_H
#define LINEBUF_H

/*
Line buffer data structure used to prevent constant write() commands to STDOUT
Linebuf has appending functionality which checks for buffer overflow
Also included are allocater and deleter for the linebuf structure
*/

struct linebuf {
	size_t s; //size of linebuffer
	size_t e; //size of escape sequences
	short int p; //pointer to last character in string

	char b[]; //buffer
};

//buffer allocater
struct linebuf * lbAllocate(size_t buf_size);

//reset linebuffer:
void lbReset(struct linebuf *lb);

//appending function
//usage: 
//for the function to work as defined,
//calls the the appending function should not mix escape sequences and text
//if this is required, these must be in two separate calls to the append function
void lbAppend(struct linebuf *lb , const char *s , int len);

//to avoid constant use of memset use ListAppend:

void lbListAppend(struct linebuf *lb , const char s , int len);

//buffer destructor
void lbFree(struct linebuf *lb);

#endif



