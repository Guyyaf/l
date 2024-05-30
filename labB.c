#include <stdio.h>
#include <stdlib.h>


void PrintHex(unsigned char *buffer, int length){
    for (int i = 0; i < length; ++i){
        printf("%X ", buffer[i]);
    }
    printf("\n");
}

int main(int argc, char **argv){
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL){
        perror("Error opening file");
        exit(1);
    }
    unsigned char buffer[100];  
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0){
        PrintHex(buffer, bytesRead);
    }
    fclose(file);
    return 0;
}