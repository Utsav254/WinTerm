#ifndef NUMBERFORMAT_H
#define NUMBERFORMAT_H

#define epsilon 0.0000001

//simple function for rounding purposes:
double myRound(double num, int precision);

//integer to ascii string:
int itoa(long num, char* buffer , size_t n);

//double to ascii string:
int ftoa(double num , int precision, char* buffer, size_t n);

//convert file size to human readable format:
void humanReadableSize(long size, char buffer[] , int n);


void ttoa(time_t timestamp, char buffer[] , size_t n , int mode);



#endif
