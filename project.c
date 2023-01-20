#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>

#define MAX_LINE 5000
#define MAX_PATH_ADDRESS 500
#define MAX_STRING 2000

void createfile(char * path);
void add_string_to_file(char * path, char * string, int mode);
void insertstr(char * path, char * string, int line, int start);
void get_pos(char ** line, int * pos_line, int * pos_start);
void get_size(char ** line, int * size);
void get_bf(char ** line, int * type);
int file_exist(char * path);
char * get_path(char ** line);
char * get_string(char ** line);
char * read_file(char * path);
long long string_to_int(char * string, int lenght);

void createfile(char * path){
    char * path_to_make = (char *)malloc((MAX_PATH_ADDRESS + 1) * sizeof(char));
    int i = 0;
    while(*path != '\0'){
        if(*path == '\\'){
            if(*(path + 1) == '\"'){
                *(path_to_make + i) = *(path +1);
                i++;
                path += 2;
                continue;
            }
        }
        if(*path == '/'){
            *(path_to_make + i) = '\0';
            mkdir(path_to_make);
        }
        *(path_to_make + i) = *path;
        i++;
        path += 1;
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
}

void add_string_to_file(char * path, char * string, int mode){
    if(mode == 1){
        FILE *file = fopen(path, "w");
        fwrite(string, sizeof(char), strlen(string), file);
        fclose(file);
    }else if(mode == 2){
        FILE *file = fopen(path, "a");
        fwrite(string, sizeof(char), strlen(string), file);
        fclose(file);
    }
}

void insertstr(char * path, char * string, int line, int start){
    char * file_data = read_file(path);
    int i = 0;
    int line_counter = 1;
    int position_counter = 0;
    while (-1){
        if(line_counter == line && position_counter == start){
            break;
        }

        if(*(file_data + i) == 10){
            line_counter++;
            position_counter = 0;
        }

        position_counter++;
        i++;
    }
    i++;
    char temp = *(file_data + i);
    *(file_data + i) = '\0';

    add_string_to_file(path, file_data, 1);
    add_string_to_file(path, string, 2);

    *(file_data + i) = temp;
    file_data += i;

    add_string_to_file(path, file_data, 2);
}

void get_pos(char ** line, int * pos_line, int * pos_start){
    //Go to firs of addres
    *(line)+=6;

    //Analyze line and start position
    *pos_line = string_to_int(*line, 1);
    *line += 2;

    *pos_start = string_to_int(*line, 1);
    *(line) += 2;
}

void get_size(char ** line, int * size){
    //Go to firs of addres
    *(line)+=1;

    //Analyze line and start position
    *size = string_to_int(*line, 1);

    *(line) += 2;
}

void get_bf(char ** line, int * type){
    //Go to firs of addres
    *(line)+=1;

    //Analyze line and start position
    if(**line == 'f'){
        *type = 1;
    }else if(**line == 'b'){
        *type = 2;
    }

    *(line) += 2;
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
        *(line) += 1;
    }

    //Go to firs of addres
    *(line)+=8;

    //Analyze path and create folders
    char * path = (char *)malloc((MAX_PATH_ADDRESS + 1) * sizeof(char));
    int i = 0;
    while(**line != end_str_with[ending_type] && **line != '\0'){
        if(**line == '\\'){
            if(*((*line)+1) == '\\' || *((*line)+1) == '\"'){
                *(path + i) = *((*line)+1);
                i++;
                *line += 2;
                continue;
            }else if(*((*line)+1) == 'n'){
                *(path + i) = 10;
                i++;
                *line += 2;
                continue;
            }
        }
        *(path + i) = **line;
        i++;
        *(line) += 1;
    }
    *(path + i) = '\0';

    *(line) += ending_type;
    *(line) += 1;

    return path;
}

char * get_string(char ** line){
    //end types
    const char end_str_with[] = {' ', '\"'};

    //char * path = line;
    int ending_type = 0;

    //Checking end type
    if(*(*(line) + 6) == '\"'){
        ending_type = 1;
        *(line) += 1;
    }

    //Go to firs of addres
    *(line)+=6;

    //Analyze path and create folders
    char * string = (char *)malloc((MAX_STRING + 1) * sizeof(char));
    int i = 0;
    while(**line != end_str_with[ending_type] && **line != '\0'){
        if(**line == '\\'){
            if(*((*line)+1) == '\\' || *((*line)+1) == '\"'){
                *(string + i) = *((*line)+1);
                i++;
                *line += 2;
                continue;
            }else if(*((*line)+1) == 'n'){
                *(string + i) = 10;
                i++;
                *line += 2;
                continue;
            }
        }
        *(string + i) = **line;
        i++;
        *(line) += 1;
    }
    *(string + i) = '\0';

    *(line) += ending_type;
    *(line) += 1;

    return string;
}

char * read_file(char * path){
    FILE *file = fopen(path, "r");
    char * data = (char *) malloc((MAX_STRING + 1) * sizeof(char));
    *data = '1';
    int i = 0;
    while (-1) {
        *(data + i) = fgetc(file);
        if(*(data + i) == EOF){
            *(data + i) = '\0';
            break;
        }
        i++;
    }
    fclose(file);
    return data;
}

long long string_to_int(char * string, int lenght){
    long long in = 0;
    int num;
    for(int i = lenght-1; i >= 0; i--){
        char st = *(string + (lenght - i - 1));
        if(st == '1') num = 1;
        if(st == '2') num = 2;
        if(st == '3') num = 3;
        if(st == '4') num = 4;
        if(st == '5') num = 5;
        if(st == '6') num = 6;
        if(st == '7') num = 7;
        if(st == '8') num = 8;
        if(st == '9') num = 9;
        in += pow(10 , i) * num;
    }

    return in;
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
        line[strlen(line) - 1] = '\0';

        //explode line to command
        sscanf(notAnalyzed, "%s", command);
        notAnalyzed += strlen(command) + 1;

        //command switch
        if(strcmp(command, "createfile") == 0){
            char * path = get_path(&notAnalyzed);
            createfile(path);
        }else if(strcmp(command, "insertstr") == 0){
            char * path = get_path(&notAnalyzed);
            if(file_exist(path)){
                char * string = get_string(&notAnalyzed);
                int pos_line;
                int pos_start;
                get_pos(&notAnalyzed, &pos_line, &pos_start);

                insertstr(path, string, pos_line, pos_start);
            }else{
                puts("File isn't exist");
            }
        }else if(strcmp(command, "cat") == 0){
            char * path = get_path(&notAnalyzed);
            puts(read_file(path));
        }else if(strcmp(command, "removestr")){
            char * path = get_path(&notAnalyzed);
            int pos_line;
            int pos_start;
            get_pos(&notAnalyzed, &pos_line, &pos_start);
            int size;
            get_size(&notAnalyzed, &size);
            int type;
            get_bf(&notAnalyzed, &type);
        }
    }

    return 0;
}