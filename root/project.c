#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dirent.h>

int is_chert = 0;
int end_line = 0;
int count_max = 0;

void get_word(char *key){
    int key_counter = 0;
    int n = 0;
    char c;
    for(int i = 0; i < sizeof(key); i++){
        *(key + i) = '\0';
    }
    while(-1){
        c = getchar();
        if(c == ' '){
            is_chert = 1;
            return;
        }else if(c == '\n'){
            end_line = 1;
            return;
        }
        n++;
        *(key + key_counter) = c;
        key_counter++;
    }
}

int is_word(char *key, char a[]){
    int i = 0;
    int n = strlen(a);
    while(-1){
        if(*(key + i) == '\0' && i == n){
            return 1;
        }
        if(*(key + i) != a[i]){
            return 0;
        }
        i++;
    }
}

void get_filepath(char *key){
    get_word(key);
}

int main(){

    char *key = (char *)malloc(sizeof(char) * 11);

    while(-1){
        is_chert = 0;
        end_line = 0;
        count_max = 0;
        get_word(key, 11);

        if (is_word(key, "createfile")){
            int in_cot = 0;
            char c;
            while (-1) {
                c = getchar();
                if(c == ' '){
                    break;
                }
            }
            char ch;
            char path[1000];
            int i = 0;
            while (-1){
                ch = getchar();
                if(ch == "\""){
                    if(in_cot){
                        break;
                    }
                    in_cot = 1;
                }
                if(ch == "/"){
                    path[i] == '\0';
                    DIR* dir = opendir(path);
                    if (dir) {
                        continue;
                    } else if (ENOENT == errno) {
                        //mkdir(path)
                    } else {
                        printf("Can not open or create folder");
                    }
                }
                if(ch == '\n'){
                    break;
                }
                path[i] = c;
                i++;
            }
        }else{

        }


    }
    return 0;
}
