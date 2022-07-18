#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;
#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    //Check if only one command line argument was input
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    //Open memory card file ans check if it's not pointing at NULL
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    //initialize variables to be used
    BYTE buffer[BLOCK_SIZE];
    size_t bytes_read = 1;
    bool first_jpg = false;
    FILE *current_img;
    bool found_jpg = false;
    int file_number = 0;
    char file_name[8];

    //Loop trhough the file untill it reaches EOF
    while (bytes_read != 0)
    {
        bytes_read = fread(buffer, sizeof(BYTE), BLOCK_SIZE, card);

        //Check if current buffer is the beginning of a JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            found_jpg = true;

            if (!first_jpg)
            {
                first_jpg = true;
            }
            else
            {
                fclose(current_img);
            }
            sprintf(file_name, "%03i.jpg", file_number);
            current_img = fopen(file_name, "w");
            fwrite(buffer, sizeof(BYTE), bytes_read, current_img);
            file_number++;
        }
        else
        {
            if (found_jpg)
            {
                fwrite(buffer, sizeof(BYTE), bytes_read, current_img);
            }
        }
    }
    fclose(current_img);
    fclose(card);
    return 0;
}