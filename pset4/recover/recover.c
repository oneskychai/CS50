// Recovers jpeg files from raw data file of deleted photos

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // Remember image filename
    char *raw_data = argv[1];

    // Open image file
    FILE *inptr = fopen(raw_data, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", raw_data);
        return 1;
    }

    // Create data type BYTE
    typedef uint8_t BYTE;

    // Create buffer
    BYTE buffer[512];

    // Set filecount to 0
    int count = 0;

    // Set jpeg found to false
    bool jpeg_found = false;

    // Create output file
    FILE *img = NULL;

    // Read from image file until all blocks are read
    while (fread(buffer, sizeof(BYTE), 512, inptr) == 512)
    {
        // Read 1st 4 bytes of buffer and look for jpeg signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // Check to see if it's not the 1st jpeg
            if (jpeg_found == true)
            {
                // Block is beginning of new jpeg, so close file
                fclose(img);
            }
            else
            {
                // Block is start of 1st jpeg, so set jpeg_found = true
                jpeg_found = true;
            }

            // Create filename
            char filename[8];
            sprintf(filename, "%03i.jpg", count);

            // Open output file
            img = fopen(filename, "a");
            if (img == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "Could not create %s.\n", filename);
                return 3;
            }

            // Add 1 to jpeg count
            count++;
        }

        // Check to see if jpegs have been found
        if (jpeg_found == true)
        {
            // Write from buffer to file
            fwrite(&buffer, sizeof(BYTE), 512, img);
        }
    }

    // Close img file and raw_data file
    fclose(img);
    fclose(inptr);

    return 0;
}
