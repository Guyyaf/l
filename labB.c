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
    if (fread(&outputVirus->SigSize, sizeof(short), 1, file) != 1) {
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
    printf("Name: %s\n", v->virusName);
    printf("Signature Size: %d\n", v->SigSize);
    printf("Signature: ");
    for (int i = 0; i < v->SigSize; ++i) {printf("%X ", v->sig[i]);}
    printf("\n");
}

////////////////////////////////////////////////////Part 1.b/////////////////////////////////////////////////////////////////////////////////////

typedef struct link link;

struct link {
link *nextVirus;
virus *vir;
}; 

link* list_append(link* virus_list, virus* data) {
    link* newLink = (link*)malloc(sizeof(link));
    newLink->vir = data;
    newLink->nextVirus = virus_list;  
    return newLink;
}

void list_print(link *virus_list, FILE *file) {
    link current = virus_list[0];
    while(current.nextVirus != NULL) {
        printVirus2(&current, file);
        current = *current.nextVirus;
    }
}

void printVirus2(virus* v, FILE *file) {
    fprintf("Name: %s\n", v->virusName, file);
    fprintf("Signature Size: %d\n", v->SigSize, file);
    fprintf("Signature: ", file);
    for (int i = 0; i < v->SigSize; ++i) {fprintf("%X ", v->sig[i], file);}
    fprintf("\n", file);
}

void list_free(link *virus_list) {
    if (virus_list != NULL){
        list_free(virus_list->nextVirus);
        free(virus_list->vir->sig);  
        free(virus_list->vir);       
        free(virus_list);    
    }     
}

int main(int argc, char **argv){
    FILE *file = fopen("signatures-L", "rb");
    if (file == NULL) {
        perror("Error opening signatures file");
        exit(1);
    }
    char magicNumber[4];
    if (fread(magicNumber, sizeof(char), 4, file) != 4) {
        fprintf(stderr, "Error reading magic number\n");
        fclose(file);
         exit(1);
    } 
    if (magicNumber[0]!='V' || magicNumber[1]!='I' || magicNumber[2]!='R' || ((magicNumber[3]!='L') && (magicNumber[4]!='B')) ) {
        fprintf(stderr, "Invalid magic number in signatures file\n");
        fclose(file);
        exit(1);
    }
    while (!feof(file)) {
        virus *v = readVirus(file);
        printVirus(v);
        free(v->sig);
        free(v);
    }
    fclose(file);
    return 0;
}
