// Importing libraries
#include <cs50.h>
#include <stdio.h>

// Variables
int size;
string space = " ";
string block = "#";

// Declaring functions
void makeSpace(int line);
void makeBlock(int line);


int main(void)
{
    // Prompt to request size
    do
    {
        size = get_int("Height:");
    }
    while (size < 1 || size > 8);

    //Creating the pyramid
    for (int c = 1; c < size + 1; c++)
    {
        makeSpace(c);
        makeBlock(c);
        printf("  ");
        makeBlock(c);
        printf("\n");
    }
}

// Library
void makeSpace(int line)
{
    for (int s = 0; s < size - line; s++)
    {
        printf("%s", space);
    }

}


void makeBlock(int line)
{
    for (int b = 0; b < line; b++)
    {
        printf("%s", block);
    }
}