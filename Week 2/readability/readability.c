// 1 - LIBRARIES

#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// 2 - DECLARING FUNCTIONS AND VARIBALES

void count_chars(string input);
float calculate_L(int l, int w);
float calculate_S(int w, int s);
void rate_string(float L, float S);

int lenght;
int letters;
int words;
int sentences;

// 3 - MAIN FUNCTION
int main(void)
{
    // We promp to ask user his input
    string input = get_string("Text: ");
    
    //We get the lenght of the input
    lenght = strlen(input);
    
    //Getting the datas by calling count_chars
    count_chars(input);
    
    //Getting the averages by calling our 'calculate' functions
    float L = calculate_L(letters, words);
    float S = calculate_S(words, sentences);
    
    //Rating string and getting result
    rate_string(L, S);

}


// 4 - FUNCTIONS LIBRARY

// Implementing the required datas to calculate (letters, words, sentences)
void count_chars(string input)
{
    int prev_charc = ' ';
    words++;
    // Looping on each letter
    for (int i = 0; i < lenght ; i++)
    {
        int charc = input[i];
        
        //We parse the letter to uppercase
        if (isupper(charc))
        {
            charc += 32;
        }
        
        //We scrore the charcarter
        if (charc >= 'a' && charc <= 'z')
        {
            letters++;
        }
        else if (prev_charc != ' ' && charc == ' ' && i < lenght - 1)
        {
            words++;
        }
        else if (charc == '!' || charc == '.' || charc == '?')
        {
            sentences++;
        }
        
        prev_charc = charc;
    }
    if (sentences == 0)
    {
        sentences = 1;
    }
}


// Getting the averages of letters in a word & words in sentence
float calculate_L(int l, int w)
{
    float result = (l * 100.00) /  w;
    return result;
}
float calculate_S(int w, int s)
{
    float result = (s * 100.00) /  w;
    return result;
}


// Rating the string and returning the Grade with the rate rounded
void rate_string(float L, float S)
{
    float result = 0.0588 * L - 0.296 * S - 15.8;
    
    if (result < 1) 
    {
        printf("Before Grade 1\n");
    } 
    else if (result > 16)
    {
        printf("Grade 16+\n");
    }
    else 
    {
        printf("Grade %i\n", (int) round(result));
    }
}