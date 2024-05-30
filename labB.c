#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus {
unsigned short SigSize;
char virusName[16];
unsigned char* sig;
} virus;

void PrintHex(unsigned char *buffer, int length){
    for (int i = 0; i < length; ++i){
        printf("%X ", buffer[i]);
    }
    printf("\n");
}

void SetSigFileName() {
    printf("Enter a new file name");
    char input[200];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    if(rename("signatures-L",  input) != 0){
        exit(1);
    }
}

virus* readVirus(FILE *file) {
    virus* outputVirus = (virus*)malloc(sizeof(virus));
    if (fread(outputVirus->SigSize, sizeof(short), 1, file) != 1) {
        fprintf(stderr, "Error reading SigSize\n");
        free(outputVirus);
        exit(1);
    }
    if (fread(outputVirus->virusName, sizeof(char[16]), 1, file) != 1) {
        fprintf(stderr, "Error reading virusName\n");
        free(outputVirus);
        exit(1);
    }
    outputVirus->sig = (unsigned char*)malloc(outputVirus->SigSize);  
    if (fread(outputVirus->sig, outputVirus->SigSize, 1, file) != 1) {
        fprintf(stderr, "Error reading sig\n");
        free(outputVirus->sig);
        free(outputVirus);
        exit(1);
    }
    return outputVirus;
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