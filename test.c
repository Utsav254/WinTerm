#include <stdio.h>

int main() {
    char c = getchar();

    // Print the integer value of the character
    printf("character: %c\n" , c);

    // Check if it's a backspace
    if (c == '\x08') {
        printf("backspace detected\n");
    }

    return 0;
}

