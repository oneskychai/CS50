// Determines whether a number is a valid Visa, Amex, or Mastercard number
// This is an alternate version using a string input to catch leading zeros
// Original version used get_long and discarded leading zeros

// Include necessary libraries
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    // Ask user for credit card number, reject unsuitable inputs
    printf("Please enter a valid credit card number, numbers only\n");
    string num;
    int n;
    bool acceptable;
    do
    {
        num = get_string("Number: ");

        // Make sure user enters something
        n = strlen(num);
        if (n == 0)
        {
            acceptable = false;
        }
        else
        {
            acceptable = true;
        }

        // Make sure all characters are numbers
        for (int i = 0; i < n; i++)
        {
            if (isdigit(num[i]) == 0)
            {
                acceptable = false;
            }
        }
    }
    while (acceptable == false);

    // Reject numbers of inappropriate length
    if (n != 13 && n != 15 && n != 16)
    {
        printf("INVALID\n");
        return 0;
    }

    // Convert string to integer array
    for (int i = 0; i < n; i++)
    {
        num[i] -= 48;
    }

    // Calculate sum for Luhn's algorithm
    int sum = 0, parity = 1;
    for (int i = n - 1; i > -1; i--)
    {
        if (parity == 1)
        {
            sum += num[i];
        }
        else
        {
            if (num[i] < 5)
            {
                sum += 2 * num[i];
            }
            else
            {
                sum += (2 * num[i]) % 10 + 1;
            }
        }
        parity *= -1;
    }

    // Check to make sure sum is a multiple of 10
    if (sum % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // Test number lengths and first digits against card criteria
    if ((n == 13 || n == 16) && num[0] == 4)
    {
        printf("VISA\n");
        return 0;
    }
    else if (n == 15 && num[0] == 3 && (num[1] == 4 || num[1] == 7))
    {
        printf("AMEX\n");
        return 0;
    }
    else if (n == 16 && num[0] == 5 && num[1] > 0 && num[1] < 6)
    {
        printf("MASTERCARD\n");
        return 0;
    }
    else
    {
        printf("INVALID\n");
    }
    return 0;
}