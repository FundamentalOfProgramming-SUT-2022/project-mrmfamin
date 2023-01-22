#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>

#define MAX_LINE 5000
#define MAX_PATH_ADDRESS 500
#define MAX_STRING 2000
#define MAX_FIND_ARGS 7
#define MAX_FIND_INDEX 100

void createfile(char * path);
void add_string_to_file(char * path, char * string, int mode);
void insertstr(char * path, char * string, int line, int start);
void removestr(char * path, int line, int start, int size, int type);
void copystr(char * path, int line, int start, int size, int type);
void cutstr(char * path, int line, int start, int size, int type);
void pastestr(char * path, int line, int start);
void find(char ** line, char * path, char * string, int is_count, int is_byword, int is_at, int is_all);
//int find_complate(char * file_data , char * string ,int is_count ,int is_at ,int is_byword ,int is_all, int * indexes);
int find_complate(char * file_data , char * string, int * indexes);
void return_find(char * file_data, int * indexes, int counter, int is_at, int is_all, int is_byword, int is_count);
int index_to_byword(int index, char * file_data);
void get_pos(char ** line, int * pos_line, int * pos_start);
void get_size(char ** line, int * size);
void get_bf(char ** line, int * type);
int step_to(char * line, char to);
int file_exist(char * path);
int folder_exist(char * path);
int get_index_of_pos(char * path,int line,int start);
char * get_path(char ** line);
char * get_string(char ** line, int effect_on_star);
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
    int index = get_index_of_pos(path, line, start);

    char temp = *(file_data + index);
    *(file_data + index) = '\0';

    add_string_to_file(path, file_data, 1);
    add_string_to_file(path, string, 2);

    *(file_data + index) = temp;
    file_data += index;

    add_string_to_file(path, file_data, 2);
}

void removestr(char * path, int line, int start, int size, int type){
    char * file_data = read_file(path);
    int index = get_index_of_pos(path, line, start);

    if(type == 1){
        char temp = *(file_data + index);
        *(file_data + index) = '\0';

        add_string_to_file(path, file_data, 1);

        *(file_data + index) = temp;
        file_data += index + size;

        add_string_to_file(path, file_data, 2);
    }else if(type == 2){
        char temp = *(file_data + index - size);
        *(file_data + index - size) = '\0';

        add_string_to_file(path, file_data, 1);

        *(file_data + index - size) = temp;
        file_data += index;

        add_string_to_file(path, file_data, 2);
    }
}

void copystr(char * path, int line, int start, int size, int type){
    char * file_data = read_file(path);
    int index = get_index_of_pos(path, line, start);
    if(type == 1){
        file_data += index;
        *(file_data + size) = '\0';
        add_string_to_file("copycut.txt", file_data, 1);
    }else if(type == 2){
        *(file_data + index) = '\0';
        file_data += index - size;

        add_string_to_file("copycut.txt", file_data, 1);
    }
}

void cutstr(char * path, int line, int start, int size, int type){
    char * file_data = read_file(path);
    int index = get_index_of_pos(path, line, start);
    if(type == 1){
        file_data += index;
        *(file_data + size) = '\0';

        add_string_to_file("copycut.txt", file_data, 1);
    }else if(type == 2){
        *(file_data + index) = '\0';
        file_data += index - size;

        add_string_to_file("copycut.txt", file_data, 1);
    }
    file_data = read_file(path);

    if(type == 1){
        char temp = *(file_data + index);
        *(file_data + index) = '\0';

        add_string_to_file(path, file_data, 1);

        *(file_data + index) = temp;
        file_data += index + size;

        add_string_to_file(path, file_data, 2);
    }else if(type == 2){
        char temp = *(file_data + index - size);
        *(file_data + index - size) = '\0';

        add_string_to_file(path, file_data, 1);

        *(file_data + index - size) = temp;
        file_data += index;

        add_string_to_file(path, file_data, 2);
    }
}

void pastestr(char * path, int line, int start){
    char * file_data = read_file(path);
    int index = get_index_of_pos(path, line, start);

    char * Ced_text = read_file("copycut.txt");

    char temp = *(file_data + index);
    *(file_data + index) = '\0';

    add_string_to_file(path, file_data, 1);
    add_string_to_file(path, Ced_text, 2);

    *(file_data + index) = temp;
    file_data += index;

    add_string_to_file(path, file_data, 2);
}

void find(char ** line, char * path, char * string, int is_count, int is_byword, int is_at, int is_all){
    char * file_data = read_file(path);
    int * indexes = (int *)malloc((MAX_FIND_INDEX) * sizeof(int));
    int counter = find_complate(file_data, string, indexes);
    return_find(file_data, indexes, counter, is_at, is_all, is_byword, is_count);
}

/*
int count_counter = 0;
int is_first = 1;
int find_complate2(char * file_data , char * string ,int is_count ,int is_at ,int is_byword ,int is_all, int * indexes){
    int fd_counter = 0;
    int st_counter = 0;
    int index_find = 0;
    int index_word = 0;

    while(-1){
        printf("st_counter: %d, fd_counter: %d, index: %d, is_first: %d\n", st_counter, fd_counter, index_find, is_first);
        puts(file_data + fd_counter + index_find);
        puts(string + st_counter);
        if(*(string + st_counter) == '\0'){
            if(*(string + st_counter - 1) == '*' || *(file_data + fd_counter + index_find) == ' ' || *(file_data + fd_counter + index_find) == '\0'){
                puts("is valid");
                count_counter++;
                *(indexes + count_counter - 1) = index_find;
                if(*(file_data + index_find + fd_counter + 1) == '\0'){
                    return 0;
                }else{
                    index_find = fd_counter + 2;
                }
            }else{
                puts("is invalid");
                if(is_first){
                    index_find++;
                    fd_counter = 0;
                }else{
                    return 0;
                }
            }
            st_counter = 0;
            continue;
        }
        if(*(file_data + fd_counter + index_find) == '\0'){
            return 0;
        }
        if(*(file_data + fd_counter + index_find) != *(string + st_counter) && *(string + st_counter) != -100){
            index_find++;
            fd_counter = 0;
            st_counter = 0;
            continue;
        }
        if(*(file_data + fd_counter + index_find) == *(string + st_counter)){
            st_counter++;
            fd_counter++;
        }
        if(*(string + st_counter) == -100){
            int star_counter = 1;
            while (-1){
                if(*(file_data + fd_counter + star_counter + index_find) == '\0'){
                    return 0;
                }
                if(*(file_data + fd_counter + star_counter + index_find) == *(string + st_counter + 1)){
                    is_first = 0;
                    find_complate(file_data + fd_counter + star_counter + index_find, string + st_counter + 1, is_count, is_at, is_byword, is_all, indexes);
                    is_first = 1;
                }else{
                    fd_counter++;
                }
            }
        }
    }
}*/

int find_complate(char * file_data , char * string, int * indexes){
    int fd_counter = 0;
    int st_counter = 0;
    int index_find = 0;
    int counter = 0;
    while(-1){
        if(*(file_data + fd_counter) == '\0'){
            counter++;
            indexes[counter - 1] = index_find;
            return counter;
        }
        if(*(string + st_counter) == '\0'){
            if(*(file_data + fd_counter) == ' '){
                counter++;
                indexes[counter - 1] = index_find;
                fd_counter++;
                index_find = fd_counter;
            }
            st_counter = 0;
            continue;
        }
        if(*(file_data + fd_counter) != *(string + st_counter) && *(string + st_counter) != -100){
            fd_counter += step_to(file_data + fd_counter, ' ') +1;
            st_counter = 0;
            index_find = fd_counter;
            if(*(file_data + fd_counter) == '\0'){
                    return counter;
            }
        }else if(*(string + st_counter) == -100){
            int step = step_to(string + st_counter, ' ') - 1;
            int fd_step = step_to(file_data + fd_counter, ' ') - 1;
            if(step == 0){
                counter++;
                indexes[counter - 1] = index_find;
                fd_counter += step_to(file_data + fd_counter, ' ') + 1;
                st_counter = 0;
                index_find = fd_counter;
                if(*(file_data + fd_counter) == '\0'){
                    return counter;
                }
            }else{
                int valid = 1;
                for(int i = 0; i < step; i++){
                    if(*(file_data + fd_counter + fd_step - i) != *(string + st_counter + step - i)){
                        valid = 0;
                        fd_counter += step_to(file_data + fd_counter, ' ')+1;
                        st_counter = 0;
                        index_find = fd_counter;
                        if(*(file_data + fd_counter) == '\0'){
                            return counter;
                        }
                        break;
                    }
                }
                if(valid){
                    counter++;
                    indexes[counter - 1] = index_find;
                    fd_counter += step_to(file_data + fd_counter, ' ')+1;
                    st_counter = 0;
                    index_find = fd_counter;
                    if(*(file_data + fd_counter) == '\0'){
                        return counter;
                    }
                }
            }
        }else if (*(file_data + fd_counter) == *(string + st_counter)){
            fd_counter++;
            st_counter++;
        }
    }
}

void return_find(char * file_data, int * indexes, int counter, int is_at, int is_all, int is_byword, int is_count){
    //printf("count: %d, byword: %d, at: %d, all: %d\n", is_count, is_byword, is_at, is_all);
    if(is_count){
        if(is_all || is_at || is_byword){
            puts("Invalid args");
            return;
        }
        printf("%d\n", counter);
    }else if(is_byword){
        if(counter == 0){
            printf("%d\n", -1);
        }else{
            if(!is_at && !is_all){
                printf("%d\n",index_to_byword(*(indexes), file_data));
            }else{
                if(is_at){
                    printf("%d\n",index_to_byword(*(indexes + is_at - 1), file_data));
                }else if(is_all){
                    for(int i = 0; i < counter; i++){
                        printf("%d", index_to_byword(*(indexes + i), file_data));
                        if(i != counter-1){
                            printf(",");
                        }
                    }
                    printf("\n");
                }
            }
        }
    }else if(is_at){
        if(counter == 0){
            printf("%d\n", -1);
        }else{
            printf("%d\n",*(indexes + is_at - 1));
        }
    }else if(is_all){
        if(counter == 0){
            printf("%d\n", -1);
        }else{
            for(int i = 0; i < counter; i++){
                printf("%d", *(indexes + i));
                if(i != counter-1){
                    printf(",");
                }
            }
            printf("\n");
        }
    }else{
        if(counter == 0){
            printf("%d\n", -1);
        }else{
            printf("%d\n",*(indexes));
        }
    }
}

int index_to_byword(int index, char * file_data){
    int byword = 1;
    for(int i = 0; i <= index; i++){
        if(*(file_data + i) == ' '){
            byword++;
        }
    }
    return byword;
}

void get_pos(char ** line, int * pos_line, int * pos_start){
    //Go to firs of addres
    *(line)+=6;

    //Analyze line and start position
    *pos_line = string_to_int(*line, step_to(*line, ':'));
    *line += step_to(*line, ':') + 1;

    *pos_start = string_to_int(*line, step_to(*line, ' '));
    *(line) += step_to(*line, ' ') + 1;
}

void get_size(char ** line, int * size){
    //Go to firs of addres
    *(line)+=6;

    //Analyze line and start position
    *size = string_to_int(*line, step_to(*line, ' '));

    *(line) += step_to(*line, ' ') + 1;
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

int step_to(char * line, char to){
    int i = 0;
    while(-1){
        if(*(line + i) == to || *(line + i) == '\0'){
            break;
        }
        i++;
    }
    return i;
}

int file_exist(char * path){
    FILE *check = fopen(path, "r");
    if(check) {
        return 1;
    }else{
        char * path_to_check = (char *)malloc((MAX_PATH_ADDRESS + 1) * sizeof(char));
        int i = 0;
        while(*path != '\0'){
            if(*path == '\\'){
                if(*(path + 1) == '\"'){
                    *(path_to_check + i) = *(path +1);
                    i++;
                    path += 2;
                    continue;
                }
            }
            if(*path == '/'){
                *(path_to_check + i) = '\0';
                if(!folder_exist(path_to_check)){
                    puts("Invalid Address");
                    return 0;
                }
            }
            *(path_to_check + i) = *path;
            i++;
            path += 1;
        }
        puts("File isn't exist");
        return 0;
    }
}

int folder_exist(char * path){
    puts(path);
    DIR* dir = opendir(path);
    if (dir) {
        closedir(dir);
        return 1;
    } else {
        return 0;
    }
}

int get_index_of_pos(char *path,int line,int start){
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
            i++;
            continue;
        }

        position_counter++;
        i++;
    }
    return i;
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

char * get_string(char ** line, int effect_on_star){
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
            if(*((*line)+1) == '\\' || *((*line)+1) == '\"' || *((*line)+1) == '*'){
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
            puts(string);
        }
        if(effect_on_star && **line == '*'){
            *(string + i) = -100;
            i++;
            *(line) += 1;
            continue;
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
        if(st == '0') num = 0;
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
                char * string = get_string(&notAnalyzed, 0);
                int pos_line;
                int pos_start;
                get_pos(&notAnalyzed, &pos_line, &pos_start);

                insertstr(path, string, pos_line, pos_start);
            }
        }else if(strcmp(command, "cat") == 0){
            char * path = get_path(&notAnalyzed);
            if(file_exist(path)){
                puts(read_file(path));
            }
        }else if(strcmp(command, "removestr") == 0){
            char * path = get_path(&notAnalyzed);
            if(file_exist(path)) {
                int pos_line;
                int pos_start;
                get_pos(&notAnalyzed, &pos_line, &pos_start);
                int size;
                get_size(&notAnalyzed, &size);
                int type;
                get_bf(&notAnalyzed, &type);
                removestr(path, pos_line, pos_start, size, type);
            }
        }else if(strcmp(command, "copystr") == 0){
            char * path = get_path(&notAnalyzed);
            if(file_exist(path)){
                int pos_line;
                int pos_start;
                get_pos(&notAnalyzed, &pos_line, &pos_start);
                //printf("%d %d\n", pos_line, pos_start);
                int size;
                get_size(&notAnalyzed, &size);
                int type;
                get_bf(&notAnalyzed, &type);
                copystr(path, pos_line, pos_start, size, type);
            }
        }else if(strcmp(command, "cutstr") == 0){
            char * path = get_path(&notAnalyzed);
            if(file_exist(path)) {
                int pos_line;
                int pos_start;
                get_pos(&notAnalyzed, &pos_line, &pos_start);
                int size;
                get_size(&notAnalyzed, &size);
                int type;
                get_bf(&notAnalyzed, &type);
                cutstr(path, pos_line, pos_start, size, type);
            }
        }else if(strcmp(command, "pastestr") == 0){
            char * path = get_path(&notAnalyzed);
            if(file_exist(path)){
                int pos_line;
                int pos_start;
                get_pos(&notAnalyzed, &pos_line, &pos_start);
                pastestr(path, pos_line, pos_start);
            }
        }else if(strcmp(command, "find") == 0){
            char * args = (char *)malloc((MAX_FIND_ARGS + 1) * sizeof(char));
            int is_count = 0;
            int is_at = 0;
            int is_byword = 0;
            int is_all = 0;
            char * string;
            char * path;
            while (-1)
            {
                if(*notAnalyzed != '-'){
                    break;
                }
                sscanf(notAnalyzed, "%s", args);
                if(strcmp(args, "--str") == 0){
                    string = get_string(&notAnalyzed, 1);
                }else if(strcmp(args, "--file") == 0){
                    path = get_path(&notAnalyzed);
                }else{
                    while(-1){
                        if(*notAnalyzed != '-'){
                            break;
                        }
                        notAnalyzed += strlen(args) + 1;
                        if(strcmp(args, "-count") == 0){
                            is_count = 1;
                        }else if(strcmp(args, "-at") == 0){
                            is_at = string_to_int(notAnalyzed, step_to(notAnalyzed, ' '));
                            notAnalyzed += step_to(notAnalyzed, ' ') + 1;
                        }else if(strcmp(args, "-byword") == 0){
                            is_byword = 1;
                        }else if(strcmp(args, "-all") == 0){
                            is_all = 1;
                        }
                        if(*(notAnalyzed - 1) != ' '){
                            break;
                        }
                    }
                }
            }
            if(file_exist(path)){
                find(&notAnalyzed, path, string, is_count, is_byword, is_at, is_all);
            }
        }else if(strcmp(command, "replace")){
            char * args = (char *)malloc((MAX_FIND_ARGS + 1) * sizeof(char));
            int is_at = 0;
            int is_all = 0;
            char * string1;
            char * string2;
            char * path;
            while (-1)
            {
                if(*notAnalyzed != '-'){
                    break;
                }
                sscanf(notAnalyzed, "%s", args);
                if(strcmp(args, "--str1") == 0){
                    string1 = get_string(&notAnalyzed, 1);
                }else if(strcmp(args, "--str2") == 0){
                    string2 = get_string(&notAnalyzed, 1);
                }else if(strcmp(args, "--file") == 0){
                    path = get_path(&notAnalyzed);
                }else{
                    while(-1){
                        if(*notAnalyzed != '-'){
                            break;
                        }
                        notAnalyzed += strlen(args) + 1;
                        if(strcmp(args, "-at") == 0){
                            is_at = string_to_int(notAnalyzed, step_to(notAnalyzed, ' '));
                            notAnalyzed += step_to(notAnalyzed, ' ') + 1;
                        }else if(strcmp(args, "-all") == 0){
                            is_all = 1;
                        }
                        if(*(notAnalyzed - 1) != ' '){
                            break;
                        }
                    }
                }
            }
            if(file_exist(path)){
                replace(&notAnalyzed, path, string1, string2, is_at, is_all);
            }
        }
    }

    return 0;
}