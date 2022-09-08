//Tristan Shawn Den Ouden 13/3/2022
//Runoff.c

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    //Goes through the list of candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //Checks if the user entered a valid candidate
        if (!strcmp(name, candidates[i].name))
        {
            //Updates their preference
            preferences[voter][rank] = i;
            return true;
        }

    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    /*Note: current_vote =/= vote
    current vote -> the index of the current vote being looked at
    voter -> used to get the index of the current voter
    */
    int current_vote;
    //Goes through every voter
    for (int voter = 0; voter < voter_count; voter++)
    {
        //Goes through their preferences
        for (int pref = 0; pref < candidate_count; pref++)
        {
            //Update the current vote
            current_vote = preferences[voter][pref];
            //If the candidate is not eliminated
            if (candidates[current_vote].eliminated == false)
            {
                //Record a vote
                candidates[current_vote].votes += 1;
                //End the loop
                break;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    /*Checks for amount of votes needed
    Divides the voter count by 2, adds 1 & floors it to round it down
    */
    int votes_needed = floor(voter_count / 2.0 + 1);
    for (int i = 0; i < votes_needed; i++)
    {
        //If a candidate has more than or equal to the amount of needed votes to win
        if (candidates[i].votes >= votes_needed)
        {
            //Print the candidate's name
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int smallest_vote = candidates[0].votes;
    //Goes through the list of candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //If not eliminated + smaller than the current smallest vote
        if (candidates[i].eliminated == false && smallest_vote > candidates[i].votes)
        {
            //Update the smallest vote
            smallest_vote = candidates[i].votes;
        }
    }
    //Return the smallest vote
    return smallest_vote;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    //Goes through every candidate
    for (int i = 0; i < candidate_count; i++)
    {
        //If not eliminated + not equal to the smallest number
        if (candidates[i].eliminated == false && candidates[i].votes != min)
        {
            //Return false, election is not tied
            return false;
        }
    }
    //If all remaining candidates have equal votes, will return true instead
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    //Goes through the list of candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //If not eliminated + has the least amount of votes
        if (candidates[i].eliminated == false && candidates[i].votes == min)
        {
            //Eliminate the candidate
            candidates[i].eliminated = true;
            /*Note: does not end the loop here because there might
            be more candidates with an equal amount of votes*/
        }
    }
}