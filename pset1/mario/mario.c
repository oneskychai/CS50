// Prints 2 pyramids of height specified by user input

// Include necessary libraries
#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Get integer between 1 and 8 from user
    printf("Choose a height between 1 and 8, inclusive\n");
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    // Print pyramids with height input by user
    for (int i = 0; i < n; i++)
    {
        // Print descending number of spaces for each row
        int j = n - i;
        for (int k = 0; k < j - 1; k++)
        {
            printf(" ");
        }

        // Print ascending number of #s for each row
        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }

        // Print 2 spaces for each row
        printf("  ");

        // Print ascending number of #s for each row
        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }

        // Drop to next row
        printf("\n");
    }
}