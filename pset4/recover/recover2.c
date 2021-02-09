#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // Remember image filename
    char *image = argv[1];

    // Open image file
    FILE *inptr = fopen(image, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", image);
        return 1;
    }

    // Create data type BYTE
    typedef uint8_t BYTE;

    // Allocate memory for data
    BYTE(*data)[512] = calloc(7314, 512 * sizeof(BYTE));
    if (data == NULL)
    {
        fprintf(stderr, "Not enough memory to store data.\n");
        fclose(inptr);
        return 2;
    }

    // Iterate over image's blocks
    for (int i = 0; i < 7314; i++)
    {
        // Read block into data array
        fread(data[i], sizeof(BYTE), 512, inptr);
    }

    // Set jpeg counter to 0
    // Set marker for writing final jpeg
    int count = 0;
    int mark = 0;

    // Iterate over each block
    for (int i = 0; i < 7314; i++)
    {
        // Check first 4 bytes of block for jpeg signature
        if (data[i][0] == 255 && data[i][1] == 216 && data[i][2] == 255 &&
            data[i][3] > 223 && data[i][3] < 240)
        {
            // Set marker to i
            mark = i;

            // Look for beginning of next jpeg
            // Iterate over remaining blocks
            for (int j = i + 1; j < 7314; j++)
            {
                // Check first 4 bytes of block for jpeg signature
                if (data[j][0] == 255 && data[j][1] == 216 && data[j][2] == 255 &&
                    data[j][3] > 223 && data[j][3] < 240)
                {
                    // Create output file name
                    char filename[8];
                    sprintf(filename, "%03i.jpg", count);

                    // Open output file
                    FILE *outptr = fopen(filename, "w");
                    if (outptr == NULL)
                    {
                        fclose(inptr);
                        fprintf(stderr, "Could not create %s.\n", filename);
                        return 3;
                    }

                    // Write jpeg to output file
                    fwrite(data[i], sizeof(BYTE), 512 * (j - i), outptr);

                    // Close output file
                    fclose(outptr);

                    // Add 1 to count
                    count++;

                    // Set i = j - 1 and break
                    i = j - 1;
                    break;
                }
            }
        }
    }

    // Write final jpeg to output file
    // Create output file name
    char filename[8];
    sprintf(filename, "%03i.jpg", count);

    // Open output file
    FILE *outptr = fopen(filename, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s\n.", filename);
        return 3;
    }

    // Write jpeg to output file
    fwrite(data[mark], sizeof(BYTE), 512 * (7314 - mark), outptr);

    // Close output file
    fclose(outptr);

    // Free allocated memory for data
    free(data);

    // Close image file
    fclose(inptr);

    return 0;
}
