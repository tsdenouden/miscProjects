#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE");
        return 1;
    }
    //Open the file
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        printf("Image is unrecoverable.");
        return 1;
    }
    //Declare block size
    int BLOCK_SIZE = 512;

    //Jpeg number, Filename string declared, open the example file
    int jpeg_count = 0;
    char filename[] = "000.jpg";
    sprintf(filename, "%03i.jpg", jpeg_count);
    FILE *file = fopen(filename, "w");

    //Buffer
    BYTE buffer[BLOCK_SIZE];

    //Read until end of block
    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //If it's the first image
            if (jpeg_count == 0)
            {
                //Close example file & open the first file
                fclose(file);
                sprintf(filename, "%03i.jpg", jpeg_count);
                file = fopen(filename, "w");
                jpeg_count++;
            }
            else if (jpeg_count > 0)//New image
            {
                //Close previous file & open a new file
                fclose(file);
                sprintf(filename, "%03i.jpg", jpeg_count);
                file = fopen(filename, "w");
                jpeg_count++;
            }
        }
        //Write to the file
        fwrite(buffer, 1, 512, file);
    }
    //Close the files
    fclose(file);
    fclose(raw_file);
    return 0;
}
