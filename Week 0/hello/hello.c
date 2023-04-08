// Importing libraries
#include <cs50.h> 
#include <stdio.h>

int main(void)
{
    string name = get_string("What is your name?\n"); // Asking for users's name
    printf("hello, %s\n", name); // Saying hello
}