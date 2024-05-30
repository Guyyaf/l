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
    printf("Enter a new file name \n");
    char input[200];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; //makes end of string - gpt
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

void printVirus(virus* v) {
    printf("Virus Name: %s\n", v->virusName);
    printf("Virus Signature Size: %d\n", v->SigSize);
    printf("Virus Signature: ");
    for (int i = 0; i < v->SigSize; ++i) {printf("%X ", v->sig[i]);}
    printf("\n");
}

int main(int argc, char **argv){
     FILE *file = fopen("signatures.bin", "rb");
    if (file == NULL) {
        perror("Error opening signatures file");
        return 1;
    }

    // Read magic number
    char magicNumber[5];
    if (fread(magicNumber, sizeof(char), 4, file) != 4) {
        fprintf(stderr, "Error reading magic number\n");
        fclose(file);
        return 1;
    }
    magicNumber[4] = '\0';  // Null-terminate the string

    // Check magic number
    if (strcmp(magicNumber, "VIRL") != 0 && strcmp(magicNumber, "VIRB") != 0) {
        fprintf(stderr, "Invalid magic number in signatures file\n");
        fclose(file);
        return 1;
    }

    // Magic number is OK, proceed to read viruses
    while (!feof(file)) {
        virus *v = (virus*)malloc(sizeof(virus));
        if (v == NULL) {
            fprintf(stderr, "Memory allocation failed for virus\n");
            fclose(file);
            return 1;
        }

        // Read virus data
        if (fread(&(v->SigSize), sizeof(unsigned short), 1, file) != 1) {
            fprintf(stderr, "Error reading signature size\n");
            free(v);
            fclose(file);
            return 1;
        }

        if (fread(v->virusName, sizeof(char), 16, file) != 16) {
            fprintf(stderr, "Error reading virus name\n");
            free(v);
            fclose(file);
            return 1;
        }

        v->sig = (unsigned char*)malloc(v->SigSize);
        if (v->sig == NULL) {
            fprintf(stderr, "Memory allocation failed for signature\n");
            free(v);
            fclose(file);
            return 1;
        }

        if (fread(v->sig, 1, v->SigSize, file) != v->SigSize) {
            fprintf(stderr, "Error reading signature\n");
            free(v->sig);
            free(v);
            fclose(file);
            return 1;
        }

        printVirus(v);
        free(v->sig);
        free(v);
    }

    fclose(file);
    return 0;
}
