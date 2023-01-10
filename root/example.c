#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 64

int main()
{

    char line[MAX + 1];
    const char delim[2] = " ";
    char * wd;

    printf("Enter some text or type \"quit\" to end: ");

    while (fgets(line, MAX, stdin))
    {
        line[strlen(line) - 1] = ' ';   /* Replace newline character at the end with a space */
        wd = strtok(line, delim);

        while (wd != NULL)
        {
            if (strcmp(wd, "quit") == 0)
            {
                return 0;
            }
            printf("%s\n", wd);
            wd = strtok(NULL, delim);
        }
        printf("Enter some text or type \"quit\" to end: ");
    }

    return 0;
}