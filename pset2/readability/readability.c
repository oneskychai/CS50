// Determines approximate grade level necessary to comprehend text

// Include necessary libraries
#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <math.h>

// Declare functions defined below
float count_letters(string s);
float count_words(string s);
float count_sentences(string s);

int main(void)
{
    // Prompt user for text to analyze
    string text = get_string("Text: ");

    // Calculate L, S, and index i
    // Using floats for accuracy of calculations
    float L = 100 * count_letters(text) / count_words(text);
    float S = 100 * count_sentences(text) / count_words(text);
    float i = 0.0588 * L - 0.296 * S - 15.8;

    // Round i to nearest integer
    int grade = round(i);

    // Output computed grade level of text
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade > 15)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

// Counts the number of letters in a string
// Using float instead of int for count for accuracy during later calculations
float count_letters(string s)
{
    // Declare variables
    float count = 0;
    int i = 0;

    // Count the number of letters in the string
    while (s[i] != '\0')
    {
        if (isalpha(s[i]))
        {
            count++;
        }
        i++;
    }

    // Return letter count
    return count;
}

// Counts the number of words in a string
// Using float instead of int for count for accuracy during later calculations
float count_words(string s)
{
    // Declare variables
    float count = 0;
    int i = 0;

    // Count the number of spaces in the string
    while (s[i] != '\0')
    {
        if (s[i] == ' ')
        {
            count++;
        }
        i++;
    }

    // Return word count as number of spaces plus 1
    return count + 1;
}

// Counts the number of sentences in a string
// Using float instead of int for count for accuracy during later calculations
float count_sentences(string s)
{
    // Declare variables
    float count = 0;
    int i = 0;

    // Count the number of sentence ending punctuation marks
    while (s[i] != '\0')
    {
        if (s[i] == '.' || s[i] == '!' || s[i] == '?')
        {
            count++;
        }
        i++;
    }

    // Return sentence count
    return count;
}