#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

int main()
{
    FILE *file;

    char filename[15] = {0};
    char data_read[35] = {0};
    printf("Enter EGN:\n");
    scanf("%s", filename);
    if (strlen(filename) <= 10)
    {
        strcat(filename, ".txt");
        printf("%s\n", filename);
        file = fopen(filename, "r");
        if (file > 0)
        {
            while (fscanf(file, "%s", data_read) > 0)
            {
                printf("%s ", data_read);
            }
            printf("\n");
            fclose(file);
        }
        else
        {
            printf("There is no person with this EGN\n");
        }
    }
    else
    {
        printf("Too many symbols!\n");
    }
}