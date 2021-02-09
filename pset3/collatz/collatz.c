// Determines number of steps to get to 1 per Collatz conjecture

// Include necessary libraries
#include <stdio.h>
#include <cs50.h>

// Declare function
int collatz(int n);

int main(void)
{
    // Prompt user for number and return number of steps
    int n = get_int("Number: ");
    printf("Steps: %i\n", collatz(n));
}

// Define recursive Collatz function
int collatz(int n)
{
    // Base case
    if (n == 1)
        return 0;
    // Even numbers
    else if (n % 2 == 0)
        return collatz(n / 2) + 1;
    // Odd numbers
    else 
        return collatz(3 * n + 1) + 1;
}