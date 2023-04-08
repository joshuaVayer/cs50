#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    char winner[100];
    int score = 0;
    
    // Logical tests for each candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //In case this candidate has the higher temporary score
        if (candidates[i].votes > score)
        {
            score = candidates[i].votes;
            strcpy(winner, candidates[i].name);
        }
        // In case this candidate has score equals to the previous one
        else if (candidates[i].votes == score)
        {
            strcpy(winner, strcat(winner, "\n"));
            strcpy(winner, strcat(winner, candidates[i].name));
        }
    }
    // Print the winner(s)
    if (score > 0)
    {
        printf("%s\n", winner);
    }
    //If there is no votes
    else
    {
        printf("There is no winner"); 
    }
    
    return;
}

