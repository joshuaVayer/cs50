// Importing libraries
#include <cs50.h>
#include <stdio.h>
#include <math.h>

//Variables
float change;
int coins;


// Main function
int main(void)
{

    do
    {
        // Asking user the change
        change = get_float("Change owed:");
    }
    while (change < 0);

    // Parsing to integer
    int cents = round(change * 100);

    // Looping to find th numbers of coins
    for (int i = 0; cents >= 0; i++)
    {
        if (cents >= 25)
        {
            cents = cents - 25;
        }
        else if (cents >= 10)
        {
            cents = cents - 10;
        }
        else if (cents >= 5)
        {
            cents = cents - 5;
        }
        else
        {
            cents = cents - 1;
        }
        coins = i;
    }

    printf("%i\n", coins);
}