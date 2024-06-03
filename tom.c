[17:03, 03/06/2024] Tom Nouri: שמע פתאום קלטתי שאין לי את זה על המחשב
[17:03, 03/06/2024] Tom Nouri: זה על המחשב של האוניבריסטה
[17:03, 03/06/2024] Tom Nouri: אולי יש לי משהו בצאט
[17:04, 03/06/2024] Tom Nouri: #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 10000

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

typedef struct link {
    struct link *nextVirus;
    virus *vir;
} link;

void printVirus(virus* v);
void neutralize_virus(char *fileName, int signatureOffset);
void detect_virus(char *buffer, unsigned int size, link *virus_list);
void list_print(link *virus_list, FILE *stream);
link* list_append(link* virus_list, virus* data);
void list_free(link *virus_list);
void SetSigFileName(char* filename, size_t size);
virus* readVirus(FILE* file);

void neutralize_virus(char *fileName, int signatureOffset) {
    FILE *file = fopen(fileName, "r+b");
    if (file == NULL) {
        perror("Failed to open file for neutralizing virus");
        return;
    }

    if (fseek(file, signatureOffset, SEEK_SET) != 0) {
        perror("Failed to seek to virus location");
        fclose(file);
        return;
    }

    unsigned char retInstruction = 0xC3;
    if (fwrite(&retInstruction, 1, 1, file) != 1) {
        perror("Failed to write RET instruction to file");
    }

    fclose(file);
}

void detect_virus(char *buffer, unsigned int size, link *virus_list) {
    link *current = virus_list;
    while (current != NULL) {
        virus *v = current->vir;
        for (unsigned int i = 0; i < size - v->SigSize + 1; i++) {
            if (memcmp(&buffer[i], v->sig, v->SigSize) == 0) {
                printf("Virus detected!\n");
                printf("Starting byte location: %u\n", i);
                printf("Virus name: %s\n", v->virusName);
                printf("Virus signature size: %u\n", v->SigSize);
            }
        }
        current = current->nextVirus;
    }
}

void list_print(link *virus_list, FILE *stream) {
    link *current = virus_list;
    while (current != NULL) {
        printVirus(current->vir);
        current = current->nextVirus;
    }
}

link* list_append(link* virus_list, virus* data) {
    link new_link = (link)malloc(sizeof(link));
    if (new_link == NULL) {
        perror("Failed to allocate memory for new link");
        return virus_list;
    }
    new_link->vir = data;
    new_link->nextVirus = virus_list;
    return new_link;
}

void list_free(link *virus_list) {
    link *current = virus_list;
    while (current != NULL) {
        link *temp = current;
        current = current->nextVirus;
        free(temp->vir->sig);
        free(temp->vir);
        free(temp);
    }
}

void SetSigFileName(char* filename, size_t size) {
    printf("Enter the signature file name: ");
    fgets(filename, size, stdin);
    filename[strcspn(filename, "\n")] = '\0'; // Remove the trailing newline character
}

virus* readVirus(FILE* file) {
    virus* v = (virus*)calloc(1, sizeof(virus));
    if (v == NULL) {
        perror("Failed to allocate memory for virus");
        return NULL;
    }

    if (fread(v, 1, 18, file) != 18) {
        free(v);
        return NULL;
    }

    v->sig = (unsigned char*)calloc(v->SigSize, sizeof(unsigned char));
    if (v->sig == NULL) {
        free(v);
        perror("Failed to allocate memory for virus signature");
        return NULL;
    }

    if (fread(v->sig, 1, v->SigSize, file) != v->SigSize) {
        free(v->sig);
        free(v);
        return NULL;
    }

    return v;
}

void printVirus(virus* v) {
    if (v == NULL) return;
    printf("Virus name: %s\n", v->virusName);
    printf("Virus signature length: %u\n", v->SigSize);
    printf("Virus signature:");
    for (unsigned short i = 0; i < v->SigSize; i++) {
        printf(" %02X", v->sig[i]);
    }
    printf("\n");
}

int main() {
    char sigFileName[256] = "signatures-L";
    link *virus_list = NULL;
    char input[256];
    int option;

    while (1) {
        printf("\nMenu:\n");
        printf("0) Set signatures file name\n");
        printf("1) Load signatures\n");
        printf("2) Print signatures\n");
        printf("3) Detect viruses\n");
        printf("4) Fix file\n");
        printf("5) Quit\n");
        printf("Choose an option: ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        if (sscanf(input, "%d", &option) != 1) {
            printf("Invalid input\n");
            continue;
        }

        switch (option) {
            case 0:
                SetSigFileName(sigFileName, sizeof(sigFileName));
                break;
            case 1: {
                FILE *sigFile = fopen(sigFileName, "rb");
                if (sigFile == NULL) {
                    perror("Failed to open signature file");
                    break;
                }

                char magicNumber[4];
                if (fread(magicNumber, 1, 4, sigFile) != 4) {
                    perror("Failed to read magic number");
                    fclose(sigFile);
                    break;
                }

                if (strncmp(magicNumber, "VIRL", 4) != 0 && strncmp(magicNumber, "VIRB", 4) != 0) {
                    fprintf(stderr, "Invalid magic number\n");
                    fclose(sigFile);
                    break;
                }

                list_free(virus_list); // Free any previously loaded signatures
                virus_list = NULL;

                virus *v;
                while ((v = readVirus(sigFile)) != NULL) {
                    virus_list = list_append(virus_list, v);
                }

                fclose(sigFile);
                break;
            }
            case 2:
                list_print(virus_list, stdout);
                break;
            case 3: {
                printf("Enter the name of the suspected file: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    printf("Failed to read input\n");
                    break;
                }
                input[strcspn(input, "\n")] = '\0'; // Remove the trailing newline character

                FILE *suspectedFile = fopen(input, "rb");
                if (suspectedFile == NULL) {
                    perror("Failed to open suspected file");
                    break;
                }

                char buffer[BUFFER_SIZE];
                unsigned int size = fread(buffer, 1, BUFFER_SIZE, suspectedFile);
                fclose(suspectedFile);

                detect_virus(buffer, size, virus_list);
                break;
            }
            case 4: {
                printf("Enter the name of the infected file: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    printf("Failed to read input\n");
                    break;
                }
                input[strcspn(input, "\n")] = '\0'; // Remove the trailing newline character

                FILE *suspectedFile = fopen(input, "r+b");
                if (suspectedFile == NULL) {
                    perror("Failed to open suspected file");
                    break;
                }

                char buffer[BUFFER_SIZE];
                unsigned int size = fread(buffer, 1, BUFFER_SIZE, suspectedFile);
                rewind(suspectedFile);  // Move the file pointer back to the beginning

                link *current = virus_list;
                while (current != NULL) {
                    virus *v = current->vir;
                    for (unsigned int i = 0; i < size - v->SigSize + 1; i++) {
                        if (memcmp(&buffer[i], v->sig, v->SigSize) == 0) {
                            printf("Neutralizing virus: %s at byte location: %u\n", v->virusName, i);
                            neutralize_virus(input, i);
                        }
                    }
                    current = current->nextVirus;
                }

                fclose(suspectedFile);
                break;
            }
            case 5:
                list_free(virus_list);
                return 0;
            default:
                printf("Invalid option\n");
                break;
        }
    }

    list_free(virus_list);
    return 0;
}