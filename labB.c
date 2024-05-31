#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus {
unsigned short SigSize;
char virusName[16];
unsigned char* sig;
} virus;

typedef struct link link;

struct link {
link *nextVirus;
virus *vir;
}; 

//Global Variables
 FILE *file;
 link *virus_list;
 char *filename;

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
    if(rename(filename,  input) != 0){
        exit(1);
    }
    filename = input;
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

link* list_append(link* virus_list, virus* data) {
    link* newLink = (link*)malloc(sizeof(link));
    newLink->vir = data;
    newLink->nextVirus = virus_list;  
    return newLink;
}

void printVirus2(virus* v, FILE *file) {
    fprintf(file, "Name: %s\n", v->virusName);
    fprintf(file, "Signature Size: %d\n", v->SigSize);
    fprintf(file, "Signature: ");
    for (int i = 0; i < v->SigSize; ++i) {fprintf(file, "%X ", v->sig[i]);}
    fprintf(file, "\n");
}

void list_print(link *virus_list, FILE *file) {
    link *current = virus_list;
    while(current != NULL) {
        printVirus2(current->vir, file);
        current = current->nextVirus;
    }
}

void list_free(link *virus_list) {
    if (virus_list != NULL){
        list_free(virus_list->nextVirus);
        free(virus_list->vir->sig);  
        free(virus_list->vir);       
        free(virus_list);    
    }     
}

void loadSignatures() {
    link *newLink = virus_list;
    while (!feof(file)) {
        virus *v = readVirus(file);
        newLink = list_append(virus_list, v);
    }
    virus_list = newLink;
}

void printSignatures(){
    list_print(virus_list, stdout); 
}

void detectViruses() {
    printf("Not implemented. \n");
}

void fixFile() {
     printf("Not implemented. \n");
}

void quit() {
    exit(EXIT_SUCCESS);
}

struct fun_desc {
    char *name;
    void (*fun)();
};

struct fun_desc menu[] = { { "Set signatures file name", SetSigFileName}, { "Load signatures", loadSignatures}, { "Print signatures", printSignatures }, { "Detect viruses", detectViruses}, { "Fix file", fixFile }, { "Quit", quit}, { NULL, NULL} };

void printMenu(){
    printf("Select operation from the following menu (ctrl^D for exit):\n");
    for(int i=0; menu[i].name!=NULL; i++){
        printf("%d) %s\n",i,menu[i].name);
    }
    printf("Option: ");
}

int main(int argc, char **argv){
   file = fopen("signatures-L", "rb");
   virus_list = (link*)malloc(sizeof(link));
   char inputBuffer[100];
   int size = sizeof(menu)/sizeof(menu[0]) -1;
   printMenu();
   while(fgets(inputBuffer,100,stdin)){
        int input = atoi(inputBuffer);
        if(input<0 || input>=size){
            printf("Not within bounds\n");
            exit(1);
        }
        else{
            printf("Within Bounds\n");
            menu[input].fun();
            printf("DONE.\n");
        }
    printMenu();
    }
}













    /*
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
*/
