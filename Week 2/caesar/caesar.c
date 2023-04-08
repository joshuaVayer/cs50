// 1 - LIBRARIES
#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 2 - DECLARING FUNCTIONS AND VARIBALES
string key;
int key_int;
string input;
string output;

string cipher(string plaintext, int shift);


// 3 - MAIN FUNCTION
int main(int argc, string argv[])
{
    // We check if we have, and if it's one key entered
    if (argc == 2)
    {
        key = argv[1];
        int key_lenght = strlen(key);
        
        //We check if characters are digits
        for (int i = 0; i < key_lenght; i++)
        {
            if (key[i] < '0' || key[i] > '9')
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }

        key_int = atoi(key);

        // We ask the user for the string
        input = get_string("plaintext: ");

        // We chiper the input by calling the function
        output = cipher(input, key_int);

        // We promp the result to the user
        printf("ciphertext: %s\n", output);

    }
    // In case requirements are not matched we exit the program 
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}


// 4 - CRTYPTING FUNCTION
string cipher(string plaintext, int shift)
{
    int plaintext_lenght = strlen(plaintext);
    
    // If the key is greater than 26 we keep on with the modulo
    if (shift > 26)
    {
        shift = shift % 26;
    }

    //We loop and cipher all letters
    for (int i = 0; i < plaintext_lenght; i++)
    {
        // Uppercase letters
        if (plaintext[i] >= 'A' && plaintext[i] <= 'Z')
        {
            if (plaintext[i] + shift > 'Z')
            {
                int index = (plaintext[i] + shift) % 'Z';
                plaintext[i] = 'A' + (index - 1);
            }
            else
            {
                plaintext[i] += shift;
            }
        }
        // Lowercase letters
        else if (plaintext[i] >= 'a' && plaintext[i] <= 'z')
        {
            if (plaintext[i] + shift > 'z')
            {
                int index = (plaintext[i] + shift) % 'z';
                plaintext[i] = 'a' + (index - 1);
            }
            else
            {
                plaintext[i] += shift;
            }

        }
    }

    return plaintext;
}