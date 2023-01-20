#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_LINE 2000
#define MAX_PATH_ADDRESS 2000

char * createfile(char * line){
    //end types
    const char end_str_with[] = {' ', '\"'};

    //char * path = line;
    int ending_type = 0;

    //Checking end type
    if(*(line + 7) == '\"'){
        ending_type = 1;
        line++;
    }

    //Go to firs of addres
    line+=8;

    //Analyze path and create folders
    char * path_to_make = (char *)malloc((MAX_PATH_ADDRESS + 1) * sizeof(char));
    int i = 0;
    while(*line != end_str_with[ending_type] && *line != '\0'){
        if(*line == '/'){
            *(path_to_make + i) = '\0';
            mkdir(path_to_make);
        }
        *(path_to_make + i) = *line;
        i++;
        line++;
    }
    *(path_to_make + i) = '\0';

    //create file if not exist
    FILE *file_to_make = fopen(path_to_make, "r");
    if(file_to_make) {
        puts("File is exist");
    } else {
        file_to_make = fopen(path_to_make, "w");
    }
    fclose(file_to_make);

    line += ending_type;


    return line;
}

int main()
{
    char * line = (char *)malloc((MAX_LINE + 1) * sizeof(char));
    char * command = (char *)malloc((MAX_LINE + 1) * sizeof(char));

    while (-1)
    {
        char * notAnalyzed = line;
        fgets(line, MAX_LINE, stdin);
        line[strlen(line) - 1] = '\0'; //replace \n
        sscanf(notAnalyzed, "%s", command);
        notAnalyzed += strlen(command) + 1;
        if(strcmp(command, "createfile") == 0){
            notAnalyzed = createfile(notAnalyzed);
        }
    }

    return 0;
}