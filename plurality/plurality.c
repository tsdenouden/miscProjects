//Tristan Shawn Den Ouden 9/3/2022
//Plurality.c

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
    //Checks if the name exists in the list of candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //Compare the two strings
        if (!strcmp(name, candidates[i].name))
        {
            //Record a vote
            candidates[i].votes += 1;
            //printf("Candidate: %s Votes: %i \n", candidates[i].name, candidates[i].votes);
            return true;
        }
    }
    //If the name is not on the list, return false
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int highest_vote = 0;
    //Iterates through the list of votes.
    for (int i = 0; i < candidate_count; i++)
    {
        //Compares the current highest vote against the next vote
        if (highest_vote < candidates[i].votes)
        {
            //Updates the highest vote
            highest_vote = candidates[i].votes;
        }

    }

    //Check for ALL winners
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == highest_vote)
        {
            //Print the candidate's name
            printf("%s\n", candidates[i].name);
        }
    }
}
