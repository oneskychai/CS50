#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through each pixel of each row
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Set new color value for each pixel to average of original 3 values
            // Store sum as float to allow proper rounding
            float sum = image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed;
            int avg = round(sum / 3);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Determine midpoint of width, m is index just before midline
    // If width is odd, m will automatically be truncated to appropriate integer
    int m = width / 2 - 1;

    // Loop through each row
    for (int i = 0; i < height; i++)
    {
        // Loop through pixels in first half of each row
        for (int j = 0; j <= m; j++)
        {
            // Move RGB values to temporary location
            int tempB = image[i][j].rgbtBlue;
            int tempG = image[i][j].rgbtGreen;
            int tempR = image[i][j].rgbtRed;

            // Swap RGB values between pixels across midline
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][width - j - 1].rgbtBlue = tempB;
            image[i][width - j - 1].rgbtGreen = tempG;
            image[i][width - j - 1].rgbtRed = tempR;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Set variable w1 = width - 1;
    int w1 = width - 1;

    // Create new structure to hold sums of RGB values as floats
    // This is to prevent overflow when sums exceed 255
    // This also allows for rounding when the average is taken
    typedef struct
    {
        float b;
        float g;
        float r;
    }
    RGBSUM;

    // Create temporary array to store sums of RGB values for consecutive pixels in 3 rows
    // Each value will be the sum of the values for the current, previous, and following pixels
    RGBSUM sums[3][width];

    // Fill 1st 2 rows of sums
    for (int i = 0; i < 2; i++)
    {
        // Set 1st pixel's values
        sums[i][0].b = image[i][0].rgbtBlue + image[i][1].rgbtBlue;
        sums[i][0].g = image[i][0].rgbtGreen + image[i][1].rgbtGreen;
        sums[i][0].r = image[i][0].rgbtRed + image[i][1].rgbtRed;

        // Loop from 2nd to 2nd to last pixel and set values
        for (int j = 1; j < w1; j++)
        {
            sums[i][j].b = image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue;
            sums[i][j].g = image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen;
            sums[i][j].r = image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed;
        }

        // Set last pixel's values
        sums[i][w1].b = image[i][w1 - 1].rgbtBlue + image[i][w1].rgbtBlue;
        sums[i][w1].g = image[i][w1 - 1].rgbtGreen + image[i][w1].rgbtGreen;
        sums[i][w1].r = image[i][w1 - 1].rgbtRed + image[i][w1].rgbtRed;
    }

    // Replace first row of image

    // Replace values for first pixel
    image[0][0].rgbtBlue = round((sums[0][0].b + sums[1][0].b) / 4);
    image[0][0].rgbtGreen = round((sums[0][0].g + sums[1][0].g) / 4);
    image[0][0].rgbtRed = round((sums[0][0].r + sums[1][0].r) / 4);

    // Replace values for 2nd through 2nd to last pixel
    for (int i = 1; i < width - 1; i++)
    {
        image[0][i].rgbtBlue = round((sums[0][i].b + sums[1][i].b) / 6);
        image[0][i].rgbtGreen = round((sums[0][i].g + sums[1][i].g) / 6);
        image[0][i].rgbtRed = round((sums[0][i].r + sums[1][i].r) / 6);
    }

    // Replace values for last pixel
    image[0][w1].rgbtBlue = round((sums[0][w1].b + sums[1][w1].b) / 4);
    image[0][w1].rgbtGreen = round((sums[0][w1].g + sums[1][w1].g) / 4);
    image[0][w1].rgbtRed = round((sums[0][w1].r + sums[1][w1].r) / 4);

    // Loop from 2nd row to 2nd to last row of image
    for (int i = 1, h1 = height - 1; i < h1; i++)
    {
        // Set variable s for obsolete row of sums
        // Set variable i1 = i + 1
        int s = (i + 1) % 3;
        int i1 = i + 1;

        // Write next row of sums over obselete row

        // Write 1st pixel's values
        sums[s][0].b = image[i1][0].rgbtBlue + image[i1][1].rgbtBlue;
        sums[s][0].g = image[i1][0].rgbtGreen + image[i1][1].rgbtGreen;
        sums[s][0].r = image[i1][0].rgbtRed + image[i1][1].rgbtRed;

        // Loop from 2nd to 2nd to last pixel and set values
        for (int j = 1; j < w1; j++)
        {
            sums[s][j].b = image[i1][j - 1].rgbtBlue + image[i1][j].rgbtBlue + image[i1][j + 1].rgbtBlue;
            sums[s][j].g = image[i1][j - 1].rgbtGreen + image[i1][j].rgbtGreen + image[i1][j + 1].rgbtGreen;
            sums[s][j].r = image[i1][j - 1].rgbtRed + image[i1][j].rgbtRed + image[i1][j + 1].rgbtRed;
        }

        // Write last pixel's values
        sums[s][w1].b = image[i1][w1 - 1].rgbtBlue + image[i1][w1].rgbtBlue;
        sums[s][w1].g = image[i1][w1 - 1].rgbtGreen + image[i1][w1].rgbtGreen;
        sums[s][w1].r = image[i1][w1 - 1].rgbtRed + image[i1][w1].rgbtRed;

        // Replace next row of image

        // Write 1st pixel's values
        image[i][0].rgbtBlue = round((sums[0][0].b + sums[1][0].b + sums[2][0].b) / 6);
        image[i][0].rgbtGreen = round((sums[0][0].g + sums[1][0].g + sums[2][0].g) / 6);
        image[i][0].rgbtRed = round((sums[0][0].r + sums[1][0].r + sums[2][0].r) / 6);

        // Loop from 2nd to 2nd to last pixel
        for (int j = 1; j < width - 1; j++)
        {
            image[i][j].rgbtBlue = round((sums[0][j].b + sums[1][j].b + sums[2][j].b) / 9);
            image[i][j].rgbtGreen = round((sums[0][j].g + sums[1][j].g + sums[2][j].g) / 9);
            image[i][j].rgbtRed = round((sums[0][j].r + sums[1][j].r + sums[2][j].r) / 9);
        }

        // Write last pixel's values
        image[i][w1].rgbtBlue = round((sums[0][w1].b + sums[1][w1].b + sums[2][w1].b) / 6);
        image[i][w1].rgbtGreen = round((sums[0][w1].g + sums[1][w1].g + sums[2][w1].g) / 6);
        image[i][w1].rgbtRed = round((sums[0][w1].r + sums[1][w1].r + sums[2][w1].r) / 6);
    }

    // Replace last row of image

    // Set s1 and s2 for relevant rows of sums
    // Set h1 = height - 1
    int s1 = (height - 1) % 3;
    int s2 = (height - 2) % 3;
    int h1 = height - 1;

    // Replace values for first pixel
    image[h1][0].rgbtBlue = round((sums[s1][0].b + sums[s2][0].b) / 4);
    image[h1][0].rgbtGreen = round((sums[s1][0].g + sums[s2][0].g) / 4);
    image[h1][0].rgbtRed = round((sums[s1][0].r + sums[s2][0].r) / 4);

    // Replace values for 2nd through 2nd to last pixel
    for (int i = 1; i < width - 1; i++)
    {
        image[h1][i].rgbtBlue = round((sums[s1][i].b + sums[s2][i].b) / 6);
        image[h1][i].rgbtGreen = round((sums[s1][i].g + sums[s2][i].g) / 6);
        image[h1][i].rgbtRed = round((sums[s1][i].r + sums[s2][i].r) / 6);
    }

    // Replace values for last pixel
    image[h1][w1].rgbtBlue = round((sums[s1][w1].b + sums[s2][w1].b) / 4);
    image[h1][w1].rgbtGreen = round((sums[s1][w1].g + sums[s2][w1].g) / 4);
    image[h1][w1].rgbtRed = round((sums[s1][w1].r + sums[s2][w1].r) / 4);

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Set variable w1 = width - 1;
    int w1 = width - 1;

    // Create new structure to hold sums of RGB values as floats
    // This is to prevent overflow when sums exceed 255
    // This also allows for rounding after applying the Sobel algorithm
    typedef struct
    {
        float b;
        float g;
        float r;
    }
    RGBSUM;

    // Create arrays Gx and Gy to store values for 2 lines of image
    RGBSUM Gx[2][width];
    RGBSUM Gy[2][width];

    // Fill 1st row of Gx and Gy

    // Fill 1st pixel
    Gx[0][0].b = 2 * image[0][1].rgbtBlue + image[1][1].rgbtBlue;
    Gx[0][0].g = 2 * image[0][1].rgbtGreen + image[1][1].rgbtGreen;
    Gx[0][0].r = 2 * image[0][1].rgbtRed + image[1][1].rgbtRed;
    Gy[0][0].b = 2 * image[1][0].rgbtBlue + image[1][1].rgbtBlue;
    Gy[0][0].g = 2 * image[1][0].rgbtGreen + image[1][1].rgbtGreen;
    Gy[0][0].r = 2 * image[1][0].rgbtRed + image[1][1].rgbtRed;

    // Loop from 2nd to 2nd to last pixel
    for (int i = 1; i < w1; i++)
    {
        Gx[0][i].b = 2 * image[0][i + 1].rgbtBlue + image[1][i + 1].rgbtBlue
                     - 2 * image[0][i - 1].rgbtBlue - image[1][i - 1].rgbtBlue;
        Gx[0][i].g = 2 * image[0][i + 1].rgbtGreen + image[1][i + 1].rgbtGreen
                     - 2 * image[0][i - 1].rgbtGreen - image[1][i - 1].rgbtGreen;
        Gx[0][i].r = 2 * image[0][i + 1].rgbtRed + image[1][i + 1].rgbtRed
                     - 2 * image[0][i - 1].rgbtRed - image[1][i - 1].rgbtRed;
        Gy[0][i].b = image[1][i - 1].rgbtBlue + 2 * image[1][i].rgbtBlue + image[1][i + 1].rgbtBlue;
        Gy[0][i].g = image[1][i - 1].rgbtGreen + 2 * image[1][i].rgbtGreen + image[1][i + 1].rgbtGreen;
        Gy[0][i].r = image[1][i - 1].rgbtRed + 2 * image[1][i].rgbtRed + image[1][i + 1].rgbtRed;
    }

    // Fill last pixel
    Gx[0][w1].b = -2 * image[0][w1 - 1].rgbtBlue - image[1][w1 - 1].rgbtBlue;
    Gx[0][w1].g = -2 * image[0][w1 - 1].rgbtGreen - image[1][w1 - 1].rgbtGreen;
    Gx[0][w1].r = -2 * image[0][w1 - 1].rgbtRed - image[1][w1 - 1].rgbtRed;
    Gy[0][w1].b = 2 * image[1][w1].rgbtBlue + image[1][w1 - 1].rgbtBlue;
    Gy[0][w1].g = 2 * image[1][w1].rgbtGreen + image[1][w1 - 1].rgbtGreen;
    Gy[0][w1].r = 2 * image[1][w1].rgbtRed + image[1][w1 - 1].rgbtRed;

    // Loop from 2nd to 2nd to last row of image
    for (int i = 1; i < height - 1; i++)
    {
        // Set variable g = i % 2 to track which row of Gx and Gy to overwrite
        int g = i % 2;

        // Write values of Gx and Gy for next row

        // Write values for 1st pixel of row
        Gx[g][0].b = image[i - 1][1].rgbtBlue + 2 * image[i][1].rgbtBlue + image[i + 1][1].rgbtBlue;
        Gx[g][0].g = image[i - 1][1].rgbtGreen + 2 * image[i][1].rgbtGreen + image[i + 1][1].rgbtGreen;
        Gx[g][0].r = image[i - 1][1].rgbtRed + 2 * image[i][1].rgbtRed + image[i + 1][1].rgbtRed;
        Gy[g][0].b = 2 * image[i + 1][0].rgbtBlue + image[i + 1][1].rgbtBlue
                     - 2 * image[i - 1][0].rgbtBlue - image[i - 1][1].rgbtBlue;
        Gy[g][0].g = 2 * image[i + 1][0].rgbtGreen + image[i + 1][1].rgbtGreen
                     - 2 * image[i - 1][0].rgbtGreen - image[i - 1][1].rgbtGreen;
        Gy[g][0].r = 2 * image[i + 1][0].rgbtRed + image[i + 1][1].rgbtRed
                     - 2 * image[i - 1][0].rgbtRed - image[i - 1][1].rgbtRed;

        // Loop from 2nd to 2nd to last pixel of row and set values
        for (int j = 1; j < w1; j++)
        {
            Gx[g][j].b = image[i - 1][j + 1].rgbtBlue + 2 * image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue
                         - image[i - 1][j - 1].rgbtBlue - 2 * image[i][j - 1].rgbtBlue - image[i + 1][j - 1].rgbtBlue;
            Gx[g][j].g = image[i - 1][j + 1].rgbtGreen + 2 * image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen
                         - image[i - 1][j - 1].rgbtGreen - 2 * image[i][j - 1].rgbtGreen - image[i + 1][j - 1].rgbtGreen;
            Gx[g][j].r = image[i - 1][j + 1].rgbtRed + 2 * image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed
                         - image[i - 1][j - 1].rgbtRed - 2 * image[i][j - 1].rgbtRed - image[i + 1][j - 1].rgbtRed;
            Gy[g][j].b = image[i + 1][j - 1].rgbtBlue + 2 * image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue
                         - image[i - 1][j - 1].rgbtBlue - 2 * image[i - 1][j].rgbtBlue - image[i - 1][j + 1].rgbtBlue;
            Gy[g][j].g = image[i + 1][j - 1].rgbtGreen + 2 * image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen
                         - image[i - 1][j - 1].rgbtGreen - 2 * image[i - 1][j].rgbtGreen - image[i - 1][j + 1].rgbtGreen;
            Gy[g][j].r = image[i + 1][j - 1].rgbtRed + 2 * image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed
                         - image[i - 1][j - 1].rgbtRed - 2 * image[i - 1][j].rgbtRed - image[i - 1][j + 1].rgbtRed;
        }

        // Set values for last pixel of row
        Gx[g][w1].b = - image[i - 1][w1 - 1].rgbtBlue - 2 * image[i][w1 - 1].rgbtBlue - image[i + 1][w1 - 1].rgbtBlue;
        Gx[g][w1].g = - image[i - 1][w1 - 1].rgbtGreen - 2 * image[i][w1 - 1].rgbtGreen - image[i + 1][w1 - 1].rgbtGreen;
        Gx[g][w1].r = - image[i - 1][w1 - 1].rgbtRed - 2 * image[i][w1 - 1].rgbtRed - image[i + 1][w1 - 1].rgbtRed;
        Gy[g][w1].b = 2 * image[i + 1][w1].rgbtBlue + image[i + 1][w1 - 1].rgbtBlue
                      - 2 * image[i - 1][w1].rgbtBlue - image[i - 1][w1 - 1].rgbtBlue;
        Gy[g][w1].g = 2 * image[i + 1][w1].rgbtGreen + image[i + 1][w1 - 1].rgbtGreen
                      - 2 * image[i - 1][w1].rgbtGreen - image[i - 1][w1 - 1].rgbtGreen;
        Gy[g][w1].r = 2 * image[i + 1][w1].rgbtRed + image[i + 1][w1 - 1].rgbtRed
                      - 2 * image[i - 1][w1].rgbtRed - image[i - 1][w1 - 1].rgbtRed;

        // Calculate and write values for (i - 1)th row
        // Corresponding rows of Gx and Gy will be (i - 1) % 2

        // Set variable int Sob as placeholder for Sobel value to enable capping at 255
        int Sob;

        // Loop through each pixel of row
        for (int j = 0; j < width; j++)
        {
            // Set Sob to rounded Sobel value for Blue for pixel
            Sob = round(sqrt(pow(Gx[(i - 1) % 2][j].b, 2) + pow(Gy[(i - 1) % 2][j].b, 2)));
            if (Sob > 255)
            {
                Sob = 255;
            }

            // Set image value for Blue
            image[i - 1][j].rgbtBlue = Sob;

            // Set Sob to rounded Sobel value for Green for pixel
            Sob = round(sqrt(pow(Gx[(i - 1) % 2][j].g, 2) + pow(Gy[(i - 1) % 2][j].g, 2)));
            if (Sob > 255)
            {
                Sob = 255;
            }

            // Set image value for Green
            image[i - 1][j].rgbtGreen = Sob;

            // Set Sob to rounded Sobel value for Red for pixel
            Sob = round(sqrt(pow(Gx[(i - 1) % 2][j].r, 2) + pow(Gy[(i - 1) % 2][j].r, 2)));
            if (Sob > 255)
            {
                Sob = 255;
            }

            // Set image value for Red
            image[i - 1][j].rgbtRed = Sob;
        }
    }

    // Write final row of Gx and Gy for last row of image

    // Set variable int g = (height - 1) % 2 for rows of Gx and Gy to write on
    int g = (height - 1) % 2;

    // Write 1st pixel
    Gx[g][0].b = 2 * image[height - 1][1].rgbtBlue + image[height - 2][1].rgbtBlue;
    Gx[g][0].g = 2 * image[height - 1][1].rgbtGreen + image[height - 2][1].rgbtGreen;
    Gx[g][0].r = 2 * image[height - 1][1].rgbtRed + image[height - 2][1].rgbtRed;
    Gy[g][0].b = -2 * image[height - 2][0].rgbtBlue - image[height - 2][1].rgbtBlue;
    Gy[g][0].g = -2 * image[height - 2][0].rgbtGreen - image[height - 2][1].rgbtGreen;
    Gy[g][0].r = -2 * image[height - 2][0].rgbtRed - image[height - 2][1].rgbtRed;

    // Loop from 2nd to 2nd to last pixel
    for (int i = 1; i < w1; i++)
    {
        Gx[g][i].b = 2 * image[height - 1][i + 1].rgbtBlue + image[height - 2][i + 1].rgbtBlue
                     - 2 * image[height - 1][i - 1].rgbtBlue - image[height - 2][i - 1].rgbtBlue;
        Gx[g][i].g = 2 * image[height - 1][i + 1].rgbtGreen + image[height - 2][i + 1].rgbtGreen
                     - 2 * image[height - 1][i - 1].rgbtGreen - image[height - 2][i - 1].rgbtGreen;
        Gx[g][i].r = 2 * image[height - 1][i + 1].rgbtRed + image[height - 2][i + 1].rgbtRed
                     - 2 * image[height - 1][i - 1].rgbtRed - image[height - 2][i - 1].rgbtRed;
        Gy[g][i].b = - image[height - 2][i - 1].rgbtBlue - 2 * image[height - 2][i].rgbtBlue
                     - image[height - 2][i + 1].rgbtBlue;
        Gy[g][i].g = - image[height - 2][i - 1].rgbtGreen - 2 * image[height - 2][i].rgbtGreen
                     - image[height - 2][i + 1].rgbtGreen;
        Gy[g][i].r = - image[height - 2][i - 1].rgbtRed - 2 * image[height - 2][i].rgbtRed
                     - image[height - 2][i + 1].rgbtRed;
    }

    // Write last pixel
    Gx[g][w1].b = -2 * image[height - 1][w1 - 1].rgbtBlue - image[height - 2][w1 - 1].rgbtBlue;
    Gx[g][w1].g = -2 * image[height - 1][w1 - 1].rgbtGreen - image[height - 2][w1 - 1].rgbtGreen;
    Gx[g][w1].r = -2 * image[height - 1][w1 - 1].rgbtRed - image[height - 2][w1 - 1].rgbtRed;
    Gy[g][w1].b = -2 * image[height - 2][w1].rgbtBlue - image[height - 2][w1 - 1].rgbtBlue;
    Gy[g][w1].g = -2 * image[height - 2][w1].rgbtGreen - image[height - 2][w1 - 1].rgbtGreen;
    Gy[g][w1].r = -2 * image[height - 2][w1].rgbtRed - image[height - 2][w1 - 1].rgbtRed;

    // Calculate and write the values for the last 2 rows of image
    // Corresponding rows of Gx and Gy will be i % 2

    // Set variable int Sob as placeholder for Sobel value to enable capping at 255
    int Sob;

    // Loop through last 2 rows of image
    for (int i = height - 2; i < height; i++)
    {
        // Loop through each pixel of row
        for (int j = 0; j < width; j++)
        {
            // Set Sob to rounded Sobel value for Blue for pixel
            Sob = round(sqrt(pow(Gx[i % 2][j].b, 2) + pow(Gy[i % 2][j].b, 2)));
            if (Sob > 255)
            {
                Sob = 255;
            }

            // Set image value for Blue
            image[i][j].rgbtBlue = Sob;

            // Set Sob to rounded Sobel value for Green for pixel
            Sob = round(sqrt(pow(Gx[i % 2][j].g, 2) + pow(Gy[i % 2][j].g, 2)));
            if (Sob > 255)
            {
                Sob = 255;
            }

            // Set image value for Green
            image[i][j].rgbtGreen = Sob;

            // Set Sob to rounded Sobel value for Red for pixel
            Sob = round(sqrt(pow(Gx[i % 2][j].r, 2) + pow(Gy[i % 2][j].r, 2)));
            if (Sob > 255)
            {
                Sob = 255;
            }

            // Set image value for Red
            image[i][j].rgbtRed = Sob;
        }
    }
    return;
}
