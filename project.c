#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_LINE 5000
#define MAX_PATH_ADDRESS 500
#define MAX_STRING 2000

int file_exist(char * path);
char * createfile(char * line);
char * get_path(char * line);



void createfile(char ** line){
    //end types
    const char end_str_with[] = {' ', '\"'};

    //char * path = line;
    int ending_type = 0;

    //Checking end type
    if(*(*(line) + 7) == '\"'){
        ending_type = 1;
        *(line)++;
    }

    //Go to firs of addres
    *(line)+=8;

    //Analyze path and create folders
    char * path_to_make = (char *)malloc((MAX_PATH_ADDRESS + 1) * sizeof(char));
    int i = 0;
    while(**line != end_str_with[ending_type] && **line != '\0'){
        if(**line == '/'){
            *(path_to_make + i) = '\0';
            mkdir(path_to_make);
        }
        *(path_to_make + i) = **line;
        i++;
        *(line)++;
    }
    *(path_to_make + i) = '\0';

    //create file if not exist
    FILE *file_to_make;
    if(file_exist(path_to_make)) {
        puts("File already exist");
    } else {
        file_to_make = fopen(path_to_make, "w");
    }
    fclose(file_to_make);

    *(line) += ending_type;
}

int file_exist(char * path){
    FILE *check = fopen(path, "r");
    if(check) {
        return 1;
    }else{
        return 0;
    }
}

char * get_path(char ** line){
    //end types
    const char end_str_with[] = {' ', '\"'};

    //char * path = line;
    int ending_type = 0;

    //Checking end type
    if(*(*(line) + 7) == '\"'){
        ending_type = 1;
        *(line)++;
    }

    //Go to firs of addres
    *(line)+=8;

    //Analyze path and create folders
    char * path = (char *)malloc((MAX_PATH_ADDRESS + 1) * sizeof(char));
    int i = 0;
    while(**line != end_str_with[ending_type] && **line != '\0'){
        *(path + i) = **line;
        i++;
        *(line)++;
    }
    *(path + i) = '\0';

    return path;
}\

char * get_string(char ** line){
    //end types
    const char end_str_with[] = {' ', '\"'};

    //char * path = line;
    int ending_type = 0;

    //Checking end type
    if(*(*(line) + 6) == '\"'){
        ending_type = 1;
        *(line)++;
    }

    //Go to firs of addres
    *(line)+=6;

    //Analyze path and create folders
    char * path = (char *)malloc((MAX_STRING + 1) * sizeof(char));
    int i = 0;
    while(**line != end_str_with[ending_type] && **line != '\0'){
        *(path + i) = **line;
        i++;
        *(line)++;
    }
    *(path + i) = '\0';

    return path;
}

int main()
{
    char * line = (char *)malloc((MAX_LINE + 1) * sizeof(char));
    char * command = (char *)malloc((MAX_LINE + 1) * sizeof(char));

    while (-1)
    {
        //get line
        char * notAnalyzed = line;
        fgets(line, MAX_LINE, stdin);

        //replace \n
        line[strlen(line) - 1] = '\0'

        //explode line to command
        sscanf(notAnalyzed, "%s", command);
        notAnalyzed += strlen(command) + 1;

        //command switch
        if(strcmp(command, "createfile") == 0){
            createfile(&notAnalyzed);
        }else if(strcmp(command, "insertstr") == 0){
            if(file_exist(get_path(&notAnalyzed))){
                char * string = get_string(&notAnalyzed);
                printf("%s\n", string);
            }else{
                puts("File isn't exist");
            }
        }
    }

    return 0;
}