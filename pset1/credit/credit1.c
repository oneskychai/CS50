// determines whether a number is a valid visa, amex, or mastercard number

// note that leading zeros in a number may produce a false valid response
// this is due to the fact that leading zeros are immediately discarded upon assignment of n
// for example, a 16 digit number that is a 0 followed by the 15 digits of a valid amex number
// will return the output "AMEX" even though it is technically invalid
// i see no easy way to account for this other than asking for each digit individually
// or asking for the number twice, once as a long integer, and once as a string

// include necessary libraries
#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // ask user for credit card number, reject unsuitable inputs
    printf("Please enter a valid credit card number, numbers only\n");
    long int n;
    do
    {
        n = get_long("Number: ");
    }
    while (n < 1 || n > 9999999999999999);

    // calculate s = sum for Luhn's algorithm
    // calculate x = number of digits entered
    // calculate m = 0 for odd number of digits, m = 1 for even
    // calculate a and b, first two digits of number, order dependent on m
    int s = 0, x = 0, a, b, m;
    do
    {
        // extract last digit and add to sum
        a = n % 10;
        s = s + a;

        // trim off last digit of n
        n = (n - a) / 10;

        // set number of digits to 'odd'
        m = 0;

        //increase digit count by 1
        x++;

        // determine if all digits have been accounted for
        // if so, stop the loop to preserve a and b
        if (n == 0)
        {
            break;
        }
        else
        {
            // extract last digit of n
            b = n % 10;

            // if last digit is less than 5, double and add to sum
            if (b < 5)
            {
                s = s + 2 * b;
            }

            // if last digit is 5 or more, double and add resulting digits to sum
            else
            {
                int c = (2 * b) % 10;
                s = s + c + 1;
            }

            // trim off last digit of n
            n = (n - b) / 10;

            // set number of digits to 'even'
            m = 1;

            // increase digit count by 1
            x++;
        }

    }
    // determine if all digits have been accounted for
    while (n != 0);

    // test to see if s is a multiple of 10
    // if not, output "INVALID" and end program
    if (s % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // determine e, first 2 digits of number using m, a, and b
    int e;
    if (m == 0)
    {
        e = 10 * a + b;
    }
    else
    {
        e = 10 * b + a;
    }

    // test number lengths and first 2 digits against card criteria
    if ((x == 13 || x == 16) && e > 39 && e < 50)
    {
        printf("VISA\n");
    }
    else if (x == 15 && (e == 34 || e == 37))
    {
        printf("AMEX\n");
    }
    else if (x == 16 && e > 50 && e < 56)
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
    }
}