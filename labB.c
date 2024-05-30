#include <stdio.h>
#include <stdlib.h>

// Function to print buffer content in hexadecimal format
void PrintHex(unsigned char *buffer, size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        printf("%02X ", buffer[i]);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s FILE\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    // Allocate buffer to read file content
    unsigned char buffer[16];  // Read in chunks of 16 bytes

    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        PrintHex(buffer, bytesRead);
    }

    fclose(file);
    return 0;
}