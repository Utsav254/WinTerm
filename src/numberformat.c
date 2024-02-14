#include <string.h>
#include <time.h>

#include "terminal.h"
#include "numberformat.h"

double myFloor(double x) {
    return (x >= 0.0) ? (int)x : (int)(x - 1.0);
}

double myRound(double num, int precision) {
    if(precision < 0) return (double)(int)(num + epsilon);
    else {
        double factor = 1.0;
        for (int i = 0; i < precision; ++i) {
            factor *= 10.0;
        }
        return myFloor(num * factor + 0.5) / factor;
    }
}

int itoa(long num, char* buffer , size_t n) {
    //check for zero
    if(num == 0) {
        if((int)n < 2) return 1;
        else {
            strncpy(buffer , "0\0" , 2);
            return 0;
        }
    }
    //if not zero
    int i = 0;
    while(num > 0 && i <(int) n) {
        if(i == (int)n-1) {
            return 1;
        }
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    //reverse the string
    int start = 0, end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
    //null terminate the string
    buffer[i] = '\0';
    return 0;
}

int ftoa(double num , int precision, char* buffer, size_t n) {
    //valid precision values:
    if (precision < 0 || (size_t)precision >= n) return 1;
    num = myRound(num , precision);

    if(precision == 0) return itoa((int)(myRound(num , precision) + epsilon) , buffer , n);
    char *start = buffer; //get pointer to start of buffer
    //extract the integer part of number and write to buffer
    int intPart = (int)num;
    if(itoa(intPart, buffer , n) == 1) return 1;

    //traverse buffer until null terminator reached
    while (*buffer != '\0') {
        buffer++;
    }
    *buffer = '.';//replace null string terminator with decimal mark;

    double fracPart = (num - intPart); //extract fractional part of number
    while(precision > 0) {
        if(buffer == (start + (int)n)-2) return 1;
        buffer++;
        fracPart *= 10;
        *buffer = (int)(fracPart + epsilon) + '0';
        fracPart -= (int)fracPart;
        precision--;
    }
    //null terminate the string 
    *(buffer+1) = '\0';
    return 0;
}

void humanReadableSize(long size, char buffer[] , int n) {
    const char suffixes[] = {'b' , 'K' , 'M' , 'G' , 'T' , 'P'};
    size_t suffixLen = sizeof(suffixes) / sizeof(suffixes[0]);

    int suffixIndex = 0;
    double sizeInDouble = size;

    while (sizeInDouble >= 1024 && suffixIndex < (int)suffixLen - 1) {
        sizeInDouble /= 1024;
        ++suffixIndex;
    }

    char temp[n];
    if(suffixIndex == 0) {
        ftoa(sizeInDouble , 0 , temp , n);
        int len = (int)strlen(temp);
        strncpy(buffer+((n-1)-len) , temp , len);
    }
    else {
        ftoa(sizeInDouble , n-4 , temp , n);
        if(temp[n-3] == '.') {
            strncpy(buffer+(n-4) , temp , (n-3));
        }
        else {
            strncpy(buffer , temp , (n-2));
        }
        buffer[n-2] = suffixes[suffixIndex];
    }
    buffer[n-1] = '\0';
}

void ttoa(time_t timestamp, char buffer[] , size_t n , int mode) {
    struct tm *timeinfo;
    timeinfo = localtime(&timestamp);
    
    if(mode == FULL_MODE) strftime(buffer, n, "%d/%m/%Y %H:%M", timeinfo);
    else strftime(buffer , n , "%d/%m/%y %H:%M" , timeinfo);
}
