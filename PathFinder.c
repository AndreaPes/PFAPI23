#include <stdio.h>
#include "stdlib.h"
#include "string.h"

int main() {
    FILE *file = fopen("inputFile.txt", "r");
    if (file == NULL) {
        printf("Could not open file\n");
        return 1;
    }

    char c;
    while ((c = fgetc(file)) != EOF) {
        putchar(c);
    }

    fclose(file);

    return 0;
}