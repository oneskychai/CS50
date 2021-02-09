// Converts plaintext to ciphertext using key entered in command line

// Include necessary libraries
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // Make sure key is entered as a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Make sure key is 26 characters long
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Make sure key is all alphabetical
    for (int i = 0; i < 26; i++)
    {
        if (isalpha(argv[1][i]) == 0)
        {
            printf("Key must contain only letters.\n");
            return 1;
        }
    }

    // Convert key to all uppercase
    for (int i = 0; i < 26; i++)
    {
        if (islower(argv[1][i]))
        {
            argv[1][i] = toupper(argv[1][i]);
        }
    }

    // Compare each letter to all previous letters to ensure no duplicates
    for (int i = 1; i < 26; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (argv[1][i] == argv[1][j])
            {
                printf("Key must contain each letter only once\n");
                return 1;
            }
        }
    }

    // Create encrypter array
    int encrypter[26];
    for (int i = 0; i < 26; i++)
    {
        // Find difference between key value and corresponding plaintext value
        encrypter[i] = argv[1][i] - 65 - i;
    }

    // Prompt user to input text to be encrypted
    string plaintext = get_string("plaintext: ");

    // Convert plaintext to ciphertext
    // Set n so strlen doesn't run every iteration of loop
    int n = strlen(plaintext);
    for (int i = 0; i < n; i++)
    {
        // Only convert letters
        if (isalpha(plaintext[i]))
        {
            // Determine letter and convert to ciphertext
            if (isupper(plaintext[i]))
            {
                int j = plaintext[i] - 65;
                plaintext[i] += encrypter[j];
                plaintext[i] = (char) plaintext[i];
            }
            else
            {
                int j = plaintext[i] - 97;
                plaintext[i] += encrypter[j];
                plaintext[i] = (char) plaintext[i];
            }
        }
    }

    // Output ciphertext
    printf("ciphertext: %s\n", plaintext);
}