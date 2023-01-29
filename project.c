#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>

#define MAX_LINE 20000
#define MAX_PATH_ADDRESS 5000
#define MAX_MULTIPATHS 15000
#define MAX_STRING 10000
#define MAX_FIND_ARGS 8
#define MAX_FIND_INDEX 500
#define MAX_UNDO_COMMAND 2000


void run(char * line, int is_undo);
void createfile(char * path);
void add_string_to_file(char * path, char * string, int mode);
int insertstr(char * path, char * string, int line, int start);
char * removestr(char * path, int line, int start, int size, int type);
void copystr(char * path, int line, int start, int size, int type);
char * cutstr(char * path, int line, int start, int size, int type);
void pastestr(char * path, int line, int start);
void find(char ** line, char * path, char * string, int is_count, int is_byword, int is_at, int is_all);
char * replace(char ** line, char * path, char * string1, char * string2, int is_at, int is_all);
void grep(char ** line, char ** paths, int paths_counter,char * string,int is_c,int is_l);
void undo(char * path);
void return_grep(int * indexes, int counter, char * path_name, char * file_read, int is_c, int is_l);
//int find_complate(char * file_data , char * string ,int is_count ,int is_at ,int is_byword ,int is_all, int * indexes);
int find_complate(char * file_data , char * string, int * indexes);
int replace_complate(char * file_data , char * string1, int * indexes_first, int * indexes_last);
void return_find(char * file_data, int * indexes, int counter, int is_at, int is_all, int is_byword, int is_count);
int index_to_byword(int index, char * file_data);
void get_pos(char ** line, int * pos_line, int * pos_start);
void get_size(char ** line, int * size);
void get_bf(char ** line, int * type);
int step_to(char * line, char to);
int file_exist(char * path);
int folder_exist(char * path);
void add_history(int type, char * path, int lenght, char * string, char * pos);
int get_index_of_pos(char * path,int line,int start);
char * get_pos_of_index(char * path,int index);
char * get_path(char ** line);
char * get_string(char ** line, int effect_on_star, int offset);
char * read_file(char * path);
long long string_to_int(char * string, int lenght);
void auto_indent(char ** line, char * path);
char * auto_indent_rec(char * file_data, int index_edn, int blok_count);
int where_closed(char * file_data, int index);
int isDir(const char* fileName);
char * tree(int depth, int depth_now, char * path);


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

void add_int_to_file(char * path, int string, int mode){
    /*char * read = (char *)malloc(sizeof(int) * 1);
    fprintf()*/
    FILE *file;
    if(mode == 1){
        file = fopen(path, "w");
        fprintf(file, "%d", string);
        //fwrite(&string, sizeof(int), 1, file);
    }else if(mode == 2){
        file = fopen(path, "a");
        fprintf(file, "%d", string);
        //fwrite(&string, sizeof(int), 1, file);
    }
    fclose(file);
}

int insertstr(char * path, char * string, int line, int start){
    char * file_data = read_file(path);
    int index = get_index_of_pos(path, line, start);

    char temp = *(file_data + index);
    *(file_data + index) = '\0';

    add_string_to_file(path, file_data, 1);
    add_string_to_file(path, string, 2);

    *(file_data + index) = temp;
    file_data += index;

    add_string_to_file(path, file_data, 2);

    return strlen(string);
}

char * removestr(char * path, int line, int start, int size, int type){
    char * file_data = read_file(path);
    int index = get_index_of_pos(path, line, start);

    if(type == 1){
        char temp = *(file_data + index);
        *(file_data + index) = '\0';

        add_string_to_file(path, file_data, 1);

        *(file_data + index) = temp;
        file_data += index;
        char * ret = (char *)malloc((MAX_STRING + 1) * sizeof(char));
        for(int i = 0;i < size; i++){
            *(ret + i) = *(file_data + i);
        }
        *(ret + size) = '\0';

        file_data += size;

        add_string_to_file(path, file_data, 2);
        return ret;
    }else if(type == 2){
        char temp = *(file_data + index - size);
        *(file_data + index - size) = '\0';

        add_string_to_file(path, file_data, 1);

        *(file_data + index - size) = temp;
        file_data += index - size;
        char * ret = (char *)malloc((MAX_STRING + 1) * sizeof(char));
        for(int i = 0; i < size; i++){
            *(ret + i) = *(file_data + i);
        }
        file_data += size;

        add_string_to_file(path, file_data, 2);
        return ret;
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

char * cutstr(char * path, int line, int start, int size, int type){
    char * file_data = read_file(path);
    int index = get_index_of_pos(path, line, start);
    char * string = (char *)malloc((MAX_STRING + 1) * sizeof(char));
    if(type == 1){
        file_data += index;
        *(file_data + size) = '\0';
        for (int  i = 0; i < size; i++)
        {
            *(string + i) = *(file_data + i);
        }
        *(string + size) = '\0';
        
        add_string_to_file("copycut.txt", file_data, 1);
    }else if(type == 2){
        *(file_data + index) = '\0';
        file_data += index - size;
        for (int  i = 0; i < size; i++)
        {
            *(string + i) = *(file_data + i);
        }
        *(string + size) = '\0';

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
    return string;
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

char * replace(char ** line, char * path, char * string1, char * string2, int is_at, int is_all){
    char * old_fileData = read_file(path);
    char * file_data = read_file(path);
    int * indexes_first = (int *)malloc((MAX_FIND_INDEX) * sizeof(int));
    int * indexes_last = (int *)malloc((MAX_FIND_INDEX) * sizeof(int));
    int counter = replace_complate(file_data, string1, indexes_first, indexes_last);
    if(is_all){
        if(counter == 0){
            printf("%d\n", -1);
        }else{
            for(int i = 0; i < counter; i++){
                file_data = read_file(path);
                char tmp = *(file_data + *(indexes_first + i));
                *(file_data + *(indexes_first + i)) = '\0';
                add_string_to_file(path, file_data, 1);
                add_string_to_file(path, string2, 2);
                *(file_data + *(indexes_first + i)) = tmp;
                file_data += *(indexes_last + i) + 1;
                add_string_to_file(path, file_data, 2);
                for(int j = i+1; j < counter; j++){
                    *(indexes_first + j) -= ((*(indexes_last + i) - *(indexes_first + i)) - strlen(string2) + 1);
                    *(indexes_last + j) -= ((*(indexes_last + i) - *(indexes_first + i)) - strlen(string2) + 1);
                }
            }
        }
    }else if(is_at){
        if(counter < is_at){
            printf("%d\n", -1);
        }
        char tmp = *(file_data + *(indexes_first + is_at));
        *(file_data + *(indexes_first + is_at)) = '\0';
        add_string_to_file(path, file_data, 1);
        add_string_to_file(path, string2, 2);
        *(file_data + *(indexes_first + is_at)) = tmp;
        file_data += *(indexes_last + is_at) + 1;
        add_string_to_file(path, file_data, 2);
    }else{
        if(counter == 0){
            printf("%d\n", -1);
        }else{
            char tmp = *(file_data + *(indexes_first));
            *(file_data + *(indexes_first)) = '\0';
            add_string_to_file(path, file_data, 1);
            add_string_to_file(path, string2, 2);
            *(file_data + *(indexes_first)) = tmp;
            file_data += *(indexes_last) + 1;
            add_string_to_file(path, file_data, 2);
        }
    }
    puts("success");
    return old_fileData;
}

void grep(char ** line, char ** paths, int paths_counter,char * string,int is_c,int is_l){
    int * indexes;
    char * file_data;
    int fd_counter;
    int st_counter;
    int index_find;
    int counter = 0;
    for (int i = 0; i < paths_counter; i++)
    {
        file_data = read_file(*(paths + i));
        indexes = (int *)malloc((MAX_FIND_INDEX) * sizeof(int));
        fd_counter = 0;
        st_counter = 0;
        index_find = 0;
        if(!is_c){
            counter = 0;
        }
        while(-1){
            if(*(file_data + fd_counter) == '\0' && fd_counter != 0 && *(string + st_counter) == '\0'){
                counter++;
                indexes[counter - 1] = index_find;
                break;
            }
            if(*(string + st_counter) == '\0'){
                counter++;
                indexes[counter - 1] = index_find;
                fd_counter += step_to(file_data + fd_counter, 10) +1;
                index_find = fd_counter;
                st_counter = 0;
                continue;
            }
            if(*(file_data + fd_counter) != *(string + st_counter)){
                fd_counter++;
                st_counter = 0;
                if(*(file_data + fd_counter) == '\0'){
                    break;
                }
            }else if (*(file_data + fd_counter) == *(string + st_counter)){
                fd_counter++;
                st_counter++;
            }
        }
        if(!is_c){
            return_grep(indexes, counter, *(paths + i), file_data, is_c, is_l);
        }
    }
    if(is_c){
        return_grep(indexes, counter, *(paths), file_data, is_c, is_l);
    }
}

void undo(char * path){
    char * history = read_file("history.txt");
    char * history_path = (char *)malloc((MAX_PATH_ADDRESS + 1) * sizeof(char));
    while(-1){
        history_path = get_path(&history);
        if(strcmp(path, history_path) == 0){
            //history += strlen(history_path) + 1;
            //make_undo(history, i);
            int step = step_to(history, -100) + 1;
            *(history + step) = '\0';
            run(history, 1);
            *(history + step) = 10;
            history += step + 1;
            add_string_to_file("history.txt", history, 1);
            break;
        }else{
            history += step_to(history , -100) + 2;
        }
    }
}

void return_grep(int * indexes, int counter, char * path_name, char * file_read, int is_c, int is_l){
    if(is_l){
        if(counter > 0){
            int j = 0;
            while(-1){
                if(*(path_name + j) == '\0'){
                    break;
                }
                printf("%c", *(path_name + j));
                j++;
            }
            printf("\n");
        }
    }else if(is_c){
        printf("%d\n", counter);
    }else{
        for(int i = 0; i < counter; i++){
            int j = 0;
            while(-1){
                if(*(path_name + j) == '\0'){
                    break;
                }
                printf("%c", *(path_name + j));
                j++;
            }
            printf(": ");
            j = 0;
            while (-1)
            {
                if(*(file_read + j + *(indexes + i)) == '\n' || *(file_read + j) == '\0'){
                    break;
                }
                printf("%c", *(file_read + j + *(indexes + i)));
                j++;
            }
            printf("\n");
        }
    }
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
        if(*(file_data + fd_counter) == '\0' && fd_counter != 0 && *(string + st_counter) == '\0'){
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

int replace_complate(char * file_data , char * string1, int * indexes_first, int * indexes_last){
    int fd_counter = 0;
    int st_counter = 0;
    int index_find = 0;
    int counter = 0;
    while(-1){
        if(*(file_data + fd_counter) == '\0' && fd_counter != 0 && *(string1 + st_counter) == '\0'){
            counter++;
            indexes_first[counter - 1] = index_find;
            indexes_last[counter -1] = fd_counter - 1;
            return counter;
        }
        if(*(string1 + st_counter) == '\0'){
            if(*(file_data + fd_counter) == ' '){
                counter++;
                indexes_first[counter - 1] = index_find;
                indexes_last[counter -1] = fd_counter - 1;
                fd_counter++;
                index_find = fd_counter;
            }
            st_counter = 0;
            continue;
        }
        if(*(file_data + fd_counter) != *(string1 + st_counter) && *(string1 + st_counter) != -100){
            fd_counter += step_to(file_data + fd_counter, ' ') +1;
            st_counter = 0;
            index_find = fd_counter;
            if(*(file_data + fd_counter) == '\0'){
                    return counter;
            }
        }else if(*(string1 + st_counter) == -100){
            int step = step_to(string1 + st_counter, ' ') - 1;
            int fd_step = step_to(file_data + fd_counter, ' ') - 1;
            if(step == 0){
                counter++;
                indexes_first[counter - 1] = index_find;
                fd_counter += step_to(file_data + fd_counter, ' ') + 1;
                indexes_last[counter -1] = fd_counter - 2;
                st_counter = 0;
                index_find = fd_counter;
                if(*(file_data + fd_counter) == '\0'){
                    return counter;
                }
            }else{
                int valid = 1;
                for(int i = 0; i < step; i++){
                    if(*(file_data + fd_counter + fd_step - i) != *(string1 + st_counter + step - i)){
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
                    indexes_first[counter - 1] = index_find;
                    fd_counter += step_to(file_data + fd_counter, ' ')+1;
                    indexes_last[counter -1] = fd_counter - 2;
                    st_counter = 0;
                    index_find = fd_counter;
                    if(*(file_data + fd_counter) == '\0'){
                        return counter;
                    }
                }
            }
        }else if (*(file_data + fd_counter) == *(string1 + st_counter)){
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
                    if(counter < is_at){
                        printf("%d\n", -1);
                    }else{
                        printf("%d\n",index_to_byword(*(indexes + is_at - 1), file_data));
                    }
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
        if(counter < is_at){
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

int multifile_exist(char ** paths, int counter){
    int ok = 1;
    for(int i = 0; i < counter; i++){
        char * path = *(paths + i);
        FILE *check = fopen(path, "r");
        if(check) {
        }else{
            ok = 0;
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
                        puts("Some addres is Invalid");
                        return 0;
                    }
                }
                *(path_to_check + i) = *path;
                i++;
                path += 1;
            }
        }
    }
    if(ok){
        return 1;
    }else{
        puts("Some file isn't exist");
    }
}

int folder_exist(char * path){
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

char * get_pos_of_index(char * path,int index){
    char * file_data = read_file(path);
    int i = 0;
    int line_counter = 1;
    int position_counter = 0;
    for(int i = 0; i < index; i++){

        if(*(file_data + i) == 10){
            line_counter++;
            position_counter=0;
        }else{
            position_counter++;
        }
    }
    char * intToStr = (char *) malloc((8 + 1) * sizeof(char));
    char * read = (char *) malloc((MAX_UNDO_COMMAND + 1) * sizeof(char));
    sprintf(intToStr, "%d", line_counter);
    strcpy(read, intToStr);
    strcat(read, ":");
    sprintf(intToStr, "%d", position_counter);
    strcat(read, intToStr);
    return read;
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

int get_multipath(char ** line, char ** paths){
    //end types
    const char end_str_with[] = {' ', '\"'};

    //char * path = line;
    
    //Go to firs of addres
    *(line)+=8;
    int counter = 0;
    while(-1){
        int ending_type = 0;
        if(**line  != '/' && **line != '\"'){
            return counter;
        }

        //Checking end type
        if(*(*(line)) == '\"'){
            ending_type = 1;
            *(line) += 1;
        }

        *(line) += 1;

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
        
        *(paths + counter) = path;

        *(line) += ending_type;
        *(line) += 1;
        counter++;
    }
}

char * get_string(char ** line, int effect_on_star, int offset){
    //end types
    const char end_str_with[] = {' ', '\"'};

    //char * path = line;
    int ending_type = 0;
    //Checking end type
    if(*(*(line) + 6 + offset) == '\"'){
        ending_type = 1;
        *(line) += 1;
    }

    //Go to firs of addres
    *(line)+= 6 + offset;

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

void auto_indent(char ** line, char * path){
    char * file_data = read_file(path);
    char * file_new_data = auto_indent_rec(file_data, strlen(file_data), 0);
    add_string_to_file(path, file_new_data, 1);
}

char * auto_indent_rec(char * file_data, int index_end, int blok_count){
    *(file_data + index_end) = '\0';
    char * block_white = (char *)malloc((MAX_STRING + 1) * sizeof(char));
    char * data = (char *)malloc((MAX_STRING + 1) * sizeof(char));
    strcpy(data, "");
    char * newline = (char *) malloc(2 * sizeof(char));
    *(newline) = (char)10;
    *(newline + 1) = '\0';
    for (int i = 0; i < blok_count*4; i++)
    {
        *(block_white + i) = ' ';
    }
    *(block_white + blok_count*4) = '\0';
    if(step_to(file_data, '{') == strlen(file_data)){
        int offset=0;
        while(-1){
            if(*(file_data + offset) == ' ' || *(file_data + offset) == (char)10){
                offset++;
            }else{
                break;
            }
        }
        file_data += offset;
        strcat(data, block_white);
        strcat(data, file_data);
        strcat(data, newline);
    }else{
        int offset=0;
        while(-1){
            if(*(file_data + offset) == ' ' || *(file_data + offset) == (char)10){
                offset++;
            }else{
                break;
            }
        }
        file_data += offset;
        int step_to_ac_open = step_to(file_data, '{');
        int last = -1;
        for (int i = 0; i < step_to_ac_open; i++)
        {
            if(*(file_data + i) != ' ' && *(file_data + i) != (char)10){
                last = i;
            }
        }
        strcat(data, block_white);
        char tmp = *(file_data + last + 1);
        *(file_data + last + 1) = '\0';
        strcat(data, file_data);
        if(strlen(file_data) != 0){
            strcat(data, " ");
        }
        *(file_data + last + 1) = tmp;
        strcat(data, "{");
        strcat(data, newline);
        strcat(data, auto_indent_rec(file_data + step_to_ac_open + 1, where_closed(file_data, step_to_ac_open + 1), blok_count + 1));
        strcat(data, block_white);
        strcat(data, "}");
        strcat(data, newline);
    }
    return data;
}

int where_closed(char * file_data, int index){
    int counter = 1;
    int i = 0;
    while(-1){
        //puts(file_data + index + i);
        if(*(file_data + index + i) == '{'){
            counter++;
        }else if(*(file_data + index + i) == '}'){
            counter--;
        }
        if(counter == 0){
            break;
        }
        i++;
    }
    int last = -1;
    for(int b = 0; b < i; b++){
        if(*(file_data + index + b) != ' ' && *(file_data + index + b) != (char)10){
            last = b;
        }
    }
    return last+1;
}

void add_history(int type, char * path, int lenght, char * string, char * pos){
    char * newline = (char *) malloc(3 * sizeof(char));
    *newline = (char)-100;
    *(newline + 1) = (char)10;
    *(newline + 2) = '\0';
    char * read = (char *) malloc((MAX_UNDO_COMMAND + 1) * sizeof(char));
    char * intToStr = (char *) malloc((8 + 1) * sizeof(char));
    switch (type)
    {
    case 1:
        strcpy(read, "--file \"/");
        strcat(read, path);
        strcat(read, "\" removestr --file \"/");
        strcat(read, path);
        strcat(read, "\" --pos ");
        strcat(read, pos);
        strcat(read, " -size ");
        sprintf(intToStr, "%d", lenght);
        strcat(read, intToStr);
        strcat(read, " -f");
        strcat(read, newline);
        insertstr("history.txt", read, 1, 0);
        break;
    
    case 2:
        strcpy(read, "--file \"/");
        strcat(read, path);
        strcat(read, "\" insertstr --file \"/");
        strcat(read, path);
        strcat(read, "\" --str ");
        strcat(read, string);
        strcat(read, " --pos ");
        strcat(read, pos);
        strcat(read, newline);
        insertstr("history.txt", read, 1, 0);
        break;

    case 3:
        strcpy(read, "--file \"/");
        strcat(read, path);
        strcat(read, "\" insertstr --file \"/");
        strcat(read, path);
        strcat(read, "\" --str ");
        strcat(read, string);
        strcat(read, " --pos ");
        strcat(read, pos);
        strcat(read, newline);
        insertstr("history.txt", read, 1, 0);
        break;

    case 4:
        strcpy(read, "--file \"/");
        strcat(read, path);
        strcat(read, "\" removestr --file \"/");
        strcat(read, path);
        strcat(read, "\" --pos ");
        strcat(read, pos);
        strcat(read, " -size ");
        sprintf(intToStr, "%d", lenght);
        strcat(read, intToStr);
        strcat(read, " -f");
        strcat(read, newline);
        insertstr("history.txt", read, 1, 0);
        break;

    case 5:
        strcpy(read, "--file \"/");
        strcat(read, path);
        strcat(read, "\" undo_replace --file \"/");
        strcat(read, path);
        strcat(read, "\" --str \"");
        strcat(read, string);
        strcat(read, "\"");
        strcat(read, newline);
        insertstr("history.txt", read, 1, 0);
        break;
    }
}

char * tree(int depth, int depth_now, char * path){
    if(depth_now-1 == depth){
        return "";
    }
    char * newline = (char *) malloc(2 * sizeof(char));
    *newline = (char)10;
    *(newline + 1) = '\0';

    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    char * direct = (char *)malloc((MAX_PATH_ADDRESS + 1) * sizeof(char));
    char * depth_them = (char *)malloc((MAX_PATH_ADDRESS + 1) * sizeof(char));
    char * data = (char *)malloc((MAX_STRING + 1) * sizeof(char));
    strcpy(data, "");
    strcpy(depth_them, "");
    for(int i = 1; i < depth_now; i++){
        strcat(depth_them, "|____ ");
    }
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            strcpy(direct, path);
            strcat(direct, dir->d_name);
            strcat(direct, "/");
            if(!dir->d_type){
                strcat(data, depth_them);
                strcat(data, dir->d_name);
                strcat(data, newline);
            }else{
                if(strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){
                    strcat(data, depth_them);
                    strcat(data, dir->d_name);
                    strcat(data, ":");
                    strcat(data, newline);
                    strcat(data, tree(depth, depth_now+1, direct));
                }
            }
        }
        closedir(d);
    }
    return data;
}

void run(char * line, int is_undo){
    char * command = (char *)malloc((MAX_LINE + 1) * sizeof(char));
    char * notAnalyzed = line;

    //explode line to command
    sscanf(notAnalyzed, "%s", command);
    notAnalyzed += strlen(command) + 1;

    //command switch
    if(strcmp(command, "undo_replace") == 0){
        char * path = get_path(&notAnalyzed);
        char * string = get_string(&notAnalyzed, 0, 0);
        add_string_to_file(path, string, 1);
    }else if(strcmp(command, "createfile") == 0){
        char * path = get_path(&notAnalyzed);
        createfile(path);
    }else if(strcmp(command, "insertstr") == 0){
        char * path = get_path(&notAnalyzed);
        if(file_exist(path)){
            char * string = get_string(&notAnalyzed, 0, 0);
            int pos_line;
            int pos_start;
            get_pos(&notAnalyzed, &pos_line, &pos_start);
            int lenght = insertstr(path, string, pos_line, pos_start);
            char * intToStr = (char *) malloc((8 + 1) * sizeof(char));
            char * pos = (char *) malloc((MAX_UNDO_COMMAND + 1) * sizeof(char));
            sprintf(intToStr, "%d", pos_line);
            strcpy(pos, intToStr);
            strcat(pos, ":");
            sprintf(intToStr, "%d", pos_start);
            strcat(pos, intToStr);
            if(!is_undo){
                add_history(1, path, lenght, "", pos);
            }
        }
    }else if(strcmp(command, "cat") == 0){
        char * path = get_path(&notAnalyzed);
        if(file_exist(path)){
            puts(read_file(path));
        }
    }else if(strcmp(command, "removestr") == 0){
        char * path = get_path(&notAnalyzed);
        char * string;
        if(file_exist(path)) {
            int pos_line;
            int pos_start;
            get_pos(&notAnalyzed, &pos_line, &pos_start);
            int size;
            get_size(&notAnalyzed, &size);
            int type;
            get_bf(&notAnalyzed, &type);
            int index = get_index_of_pos(path, pos_line, pos_start);
            if(type == 2){
                index -= size;
            }
            string = removestr(path, pos_line, pos_start, size, type);
            if(!is_undo){
                add_history(2, path, 0, string, get_pos_of_index(path, index));
            }
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
            int index = get_index_of_pos(path, pos_line, pos_start);
            if(type == 2){
                index -= size;
            }
            char * string = cutstr(path, pos_line, pos_start, size, type);
            if(!is_undo){
                add_history(3, path, 0, string, get_pos_of_index(path, index));
            }
        }
    }else if(strcmp(command, "pastestr") == 0){
        char * path = get_path(&notAnalyzed);
        if(file_exist(path)){
            int pos_line;
            int pos_start;
            get_pos(&notAnalyzed, &pos_line, &pos_start);
            int lenght = strlen(read_file("copycut.txt"));
            pastestr(path, pos_line, pos_start);
            char * intToStr = (char *) malloc((8 + 1) * sizeof(char));
            char * pos = (char *) malloc((MAX_UNDO_COMMAND + 1) * sizeof(char));
            sprintf(intToStr, "%d", pos_line);
            strcpy(pos, intToStr);
            strcat(pos, ":");
            sprintf(intToStr, "%d", pos_start);
            strcat(pos, intToStr);
            if(!is_undo){
                add_history(4, path, lenght, "", pos);
            }
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
                string = get_string(&notAnalyzed, 1, 0);
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
    }else if(strcmp(command, "replace") == 0){
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
                string1 = get_string(&notAnalyzed, 1, 1);
            }else if(strcmp(args, "--str2") == 0){
                string2 = get_string(&notAnalyzed, 1, 1);
            }else if(strcmp(args, "--file") == 0){
                path = get_path(&notAnalyzed);
            }else if(strcmp(args, "-at") == 0){
                if(is_all){
                    puts("Invalid arg. can't set -at with -all");
                }
                notAnalyzed += strlen(args) + 1;
                is_at = string_to_int(notAnalyzed, step_to(notAnalyzed, ' '));
                notAnalyzed += step_to(notAnalyzed, ' ') + 1;
            }else if(strcmp(args, "-all") == 0){
                if(is_at){
                    puts("Invalid arg. can't set -at with -all");
                }
                notAnalyzed += strlen(args) + 1;
                is_all = 1;
            }
        }
        char * old_string;
        if(file_exist(path)){
            old_string = replace(&notAnalyzed, path, string1, string2, is_at, is_all);
            if(!is_undo){
                add_history(5, path, 0, old_string, "");
            }
        }
    }else if(strcmp(command, "grep") == 0){
        char * args = (char *)malloc((MAX_FIND_ARGS + 1) * sizeof(char));
        int is_c = 0;
        int is_l = 0;
        char * string;
        char * paths[MAX_MULTIPATHS];
        int paths_counter = 0;
        while (-1)
        {
            if(*notAnalyzed != '-'){
                break;
            }
            sscanf(notAnalyzed, "%s", args);
            if(strcmp(args, "--str") == 0){
                string = get_string(&notAnalyzed, 1, 0);
            }else if(strcmp(args, "--files") == 0){
                paths_counter = get_multipath(&notAnalyzed , paths);
            }else if(strcmp(args, "-l") == 0){
                if(is_c){
                    puts("Invalid arg. can't set -c with -l");
                    continue;
                }
                notAnalyzed += strlen(args) + 1;
                is_l = 1;
            }else if(strcmp(args, "-c") == 0){
                if(is_l){
                    puts("Invalid arg. can't set -c with -l");
                    continue;
                }
                notAnalyzed += strlen(args) + 1;
                is_c = 1;
            }
        }
        if(multifile_exist(paths, paths_counter)){
            grep(&notAnalyzed, paths, paths_counter, string, is_c, is_l);
        }
    }else if(strcmp(command, "undo") == 0){
        char * path = get_path(&notAnalyzed);
        if(file_exist(path)) {
            undo(path);
        }
    }else if(strcmp(command, "auto-indent") == 0){
        char * path = get_path(&notAnalyzed);
        char * old_data = read_file(path);
        if(file_exist(path)) {
            auto_indent(&notAnalyzed, path);
        }
        if(!is_undo){
            add_history(5, path, 0, old_data, "");
        }
    }else if(strcmp(command, "tree") == 0){
        int depth;
        sscanf(notAnalyzed, "%d", &depth);
        char * trees = tree(depth, 1, "root/");
        printf("%s", trees);
    }
}

int main()
{
    char * line = (char *)malloc((MAX_LINE + 1) * sizeof(char));
    
    while (-1)
    {
        //get line
        fgets(line, MAX_LINE, stdin);
        line[strlen(line) - 1] = '\0';
        run(line, 0);
    }
    return 0;
}