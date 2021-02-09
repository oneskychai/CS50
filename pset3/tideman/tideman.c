// Determines the winner of a Tideman election

#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool find_cycle(int win, int j);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Set all elements of preferences graph to 0
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            preferences[i][j] = 0;
        }
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Loop through candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // If name matches candidate, register vote
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Loop through all pairs on one vote
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Set k = 0 to start pair count
    int k = 0;

    // Loop through all candidate pairs
    // Record winners and update pair count and pairs index
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences [j][i])
            {
                pairs[k].winner = i;
                pairs[k].loser = j;
                pair_count += 1;
                k++;
            }
            else if (preferences[i][j] < preferences [j][i])
            {
                pairs[k].winner = j;
                pairs[k].loser = i;
                pair_count += 1;
                k++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Use selection sort method
    // Loop through all but final pair
    for (int i = 0; i < pair_count - 1; i++)
    {
        // Set ith index as highest count
        int highest_count = preferences[pairs[i].winner][pairs[i].loser];
        int highest_index = i;

        // Loop through remaining pairs and record highest count and index
        for (int j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] > highest_count)
            {
                highest_count = preferences[pairs[j].winner][pairs[j].loser];
                highest_index = j;
            }
        }

        // If not there, move highest pair to ith position
        if (highest_index != i)
        {
            int win = pairs[highest_index].winner;
            int lose = pairs[highest_index].loser;
            pairs[highest_index].winner = pairs[i].winner;
            pairs[highest_index].loser = pairs[i].loser;
            pairs[i].winner = win;
            pairs[i].loser = lose;
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Loop through sorted pairs
    for (int i = 0; i < pair_count; i++)
    {
        // Make sure no cycles are created
        if (find_cycle(pairs[i].winner, pairs[i].loser) == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // Loop through candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Loop through candidates again
        for (int j = 0; j < candidate_count; j++)
        {
            // Check if anyone is locked over candidate i
            if (locked[j][i] == true)
            {
                break;
            }

            // If nobody is locked over, declare candidate i winner
            if (j == candidate_count - 1)
            {
                printf("%s\n", candidates[i]);
                return;
            }
        }
    }
    return;
}

// Examine recursively to see if a path leads back to win
bool find_cycle(int win, int j)
{
    // Loop through candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Look for locked pairs
        if (locked[j][i] == true)
        {
            // If path leads back to win, return true
            if (i == win)
            {
                return true;
            }
            else if (find_cycle(win, i) == true)
            {
                return true;
            }
        }
    }
    return false;
}