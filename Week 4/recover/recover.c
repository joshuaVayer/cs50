#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <stdint.h>

// CONST
#define BLOCK_SIZE 512
#define FILE_NAME_SIZE 8

typedef uint8_t BYTE;

// PROTOTYPE
bool new_jpeg(BYTE buffer[]);

int main(int argc, char *argv[])
{
    // Check if program is called properly
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");

    // Check if th file exist
    if (input == NULL)
    {
        printf("File not found\n");
        return 2;
    }

    BYTE buffer[BLOCK_SIZE];

    int index = 0;
    bool first_jpg_found = false;
    FILE *output;
    // Loops until we have fully read the input
    while (fread(buffer, BLOCK_SIZE, 1, input))
    {
        // Checks if it's a new file by testing the header
        if (new_jpeg(buffer))
        {
            if (!first_jpg_found)
            {
                first_jpg_found = true;
            }

            else
            {
                fclose(output);
            }
            // Creates a new output
            char filename[FILE_NAME_SIZE];
            sprintf(filename, "%03i.jpg", index++);
            output = fopen(filename, "w");

            if (output == NULL)
            {
                return 1;
            }
            // Copy the input content to the output
            fwrite(buffer, BLOCK_SIZE, 1, output);
        }

        else if (first_jpg_found)
        {
            fwrite(buffer, BLOCK_SIZE, 1, output);
        }
    }
    fclose(output);
    fclose(input);
}


bool new_jpeg(BYTE buffer[])
{
    return buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0;
}