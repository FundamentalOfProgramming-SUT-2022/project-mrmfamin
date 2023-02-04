#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_COMMAND 1000
#define MAX_STRING 20000
#define MAX_FILE_NAME 100
#define MAX_LINE 500

void print_empty_line();
void init_colors();
void set_color(int type);
void print_data();
void print_command();
void print_status();
void move_curser();

int editor_width;
int editor_height;

int command_width;
int command_height;

int status_width;
int status_height;

int mode = -1;

char * command;
char * data;
char * file_name;
char * copyed;

int max_data = 0;
int max_data_pos = 0;
int max_command = 0;

int pos_line = 1;
int pos_char = 3;

int data_pos_line = 1;
int data_pos_char = 0;

int view_line_from = 1;
int view_line_to;
int data_moved = 0;
int moved = 0;

int * line_info[2];

int is_opened_file = 0;
int is_saved_file = 0;

int data_line_counter = 1;
int line_counter = 1;

char file_data[20][70];

void print_empty_line(){
    set_color(2);
    // for (int i = 1; i < editor_height; i++)
    // {
    //     char c = mvinch(i, 0);
    //     if(c==32){
    //         move(i, 0);
    //         printw("~");
    //     }
    // }
    // move(0, 0);
    for (int i = line_counter; i < editor_height; i++)
    {
        move(i, 0);
        printw(" ~");
    }
    move_curser();
    //attroff(COLOR_PAIR(3));
}

void print_number_line(){
    set_color(1);
    for (int i = 0; i < line_counter && i < view_line_to; i++)
    {
        move(i, 0);
        printw("%d",i+1);
    }
    move_curser();
}

void move_curser(){
    move(pos_line-1, pos_char);
}

void init_colors(){
    init_color(COLOR_GREEN, 259, 510, 424);
    init_color(COLOR_BLUE, 400, 400, 400);
    init_color(COLOR_RED, 173, 173, 173);
    //init_color(COLOR_YELLOW, 0, 514, 157);

    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_YELLOW, COLOR_RED);
    init_pair(3, COLOR_WHITE, COLOR_BLUE);
    init_pair(4, COLOR_RED, COLOR_GREEN);

    wbkgd(stdscr, COLOR_PAIR(1));
}

void set_color(int type){
    attron(COLOR_PAIR(type));
}

void print_data(){
    set_color(1);
    move(40, 10);
    printw("--%d--\n", max_data);
    int print_line_counter = 1;
    int i = 3;
    int h = 0;
    while (-1)
    {
        if(h == max_data){
            break;
        }
        if(*(data + h) == 10){
            print_line_counter++;
            h++;
            i = 3;
            continue;
        }else if(i > editor_width){
            print_line_counter++;
            i = 3;
            continue;
        }
        if(print_line_counter >= view_line_from && print_line_counter <= view_line_to){
            move(print_line_counter - 1, i);
            printw("%c", *(data + h));
        }
        i++;
        h++;
    }
    move_curser();
}

void print_command(){
    set_color(1);
    move(editor_height + command_height, 0);
    printw("%s", command);
}

void print_status(){
    set_color(4);
    move(editor_height + command_height + status_height, 0);
    switch(mode){
        case 0:
            printw(" INSERT ");
            break;

        case 1:
            printw(" VISUAL ");
            break;

        case 2:
            printw(" NORMAL ");
            break;
    }
    if(is_opened_file){
        set_color(3);
        char c = is_saved_file? ' ' : '+';
        printw(" %s   %c ", file_name, c);
    }
    move_curser();
}

void command_handler(){

}

void pos_rechange(){
    int go_line = 1;
    int go_char = 3;

    int in_line = 1;
    int in_char = 0;

    int h = 0;
    while(-1){
        if(in_line == data_pos_line && in_char == data_pos_char){
            break;
        }
        if(*(data + h) == '\0'){
            break;
        }
        if(*(data + h) == 10){
            in_line++;
            in_char = 0;
            h++;
            continue;
        }
        in_char++;
        h++;
    }
    max_data_pos = h;
    int j = 0;
    while(j != h){
        // if(go_line == in_line && go_char == in_char){
        //     break;
        // }
        if(*(data + j) == '\0'){
            break;
        }
        if(*(data + j) == 10){
            go_line++;
            go_char = 3;
            j++;
            continue;
        }else if(go_char > editor_width){
            go_line++;
            go_char = 3;
            continue;
        }
        go_char++;
        j++;
    }

    pos_line = go_line;
    pos_char = go_char;
}

int main ()
{
    command = (char *) malloc((MAX_COMMAND + 1) * sizeof(char));
    data = (char *) malloc((MAX_STRING + 1) * sizeof(char));
    file_name = (char *) malloc((MAX_FILE_NAME + 1) * sizeof(char));
    copyed = (char *)malloc((MAX_STRING + 1) * sizeof(char));
    line_info[0] = (int *) malloc((MAX_LINE) * sizeof(int));
    line_info[1] = (int *) malloc((MAX_LINE) * sizeof(int));

    for(int i = 0; i < MAX_LINE; i++){
        *(line_info[0] + i) = 0;
        *(line_info[1] + i) = 3;
    }

    *file_name = '\0';

    initscr();
    clear();
    refresh();

    editor_width = getmaxx(stdscr);
    editor_height = getmaxy(stdscr);
    editor_height -= 8;
    editor_width -= 6;

    command_width = editor_width;
    status_width = editor_width;
    command_height = 1;
    status_height = 1;

    view_line_to = editor_height;

    start_color();
    init_colors();

    int input;

    while(-1)
    {
        editor_width = getmaxx(stdscr);
        editor_height = getmaxy(stdscr);
        editor_height -= 8;
        editor_width -= 6;

        command_width = editor_width;
        status_width = editor_width;
        command_height = 1;
        status_height = 1;

        clear();
        move(30, 10);
        // if(input == 10){
        //     printw("%d\n", 1000);
        // }
        //  , data: %s    , data
        // for(int i =0; i < 5; i++){
        //     printw("li[0]%d: %d\t",i+1 ,line_info[0][i]);
        //     printw("li[1]%d: %d\t",i+1 ,line_info[1][i]);
        // }
        move(32, 10);
        // printw("\n");
        printw("line_counter: %d, view_line_to: %d,data_pos_line: %d, data_pos_char: %d, mode: %d, input: %d\n", line_counter, view_line_to, data_pos_line, data_pos_char, mode, input);
        //printw("data: %s\n", data);
        refresh();

        print_empty_line();

        print_number_line();

        print_data();

        print_command();
        
        print_status();


        input = getch();
        if(mode == -1){
            if(input == (int)'i'){
                    mode = 0;
            }else if(input == (int)'v'){
                mode = 1;
            }else if(input == (int)'k'){
                if(data_pos_line > 1){
                    data_pos_line--;
                    if(*(line_info[0] + data_pos_line) < data_pos_char){
                        data_pos_char = *(line_info[0] + data_pos_line);
                    }
                }
            }else if(input == (int)'j'){
                if(data_pos_line != data_line_counter){
                    data_pos_line++;
                    if(*(line_info[0] + data_pos_line) < data_pos_char){
                        data_pos_char = *(line_info[0] + data_pos_line);
                    }
                }
            }else if(input == (int)'l'){
                if(data_pos_char != *(line_info[0] + data_pos_line)){
                    data_pos_char++;
                }
            }else if(input == (int)'h'){
                if(data_pos_char != 0){
                    data_pos_char--;
                }
            }
            if(input == (int)'/' || input == (int)':'){
                mode = 2;
                *command = input;
                *(command + 1) = '\0';
                max_command++;
            }
            pos_rechange();
        }else if(mode == 1){
            if(input == (int)'k'){
                if(data_pos_line > 1){
                    data_pos_line--;
                    if(*(line_info[0] + data_pos_line) < data_pos_char){
                        data_pos_char = *(line_info[0] + data_pos_line);
                    }
                }
            }else if(input == (int)'j'){
                if(data_pos_line != data_line_counter){
                    data_pos_line++;
                    if(*(line_info[0] + data_pos_line) < data_pos_char){
                        data_pos_char = *(line_info[0] + data_pos_line);
                    }
                }
            }else if(input == (int)'l'){
                if(data_pos_char != *(line_info[0] + data_pos_line)){
                    data_pos_char++;
                }
            }else if(input == (int)'h'){
                if(data_pos_char != 0){
                    data_pos_char--;
                }
            }else if(input == 27){
                mode = -1;
                continue;
            }else if(input == (int)'y'){
                // if()
            }
            pos_rechange();
        }else if(mode == 2){
            if(input == 10){
                max_command = 0;
                command_handler();
                *(command + max_command) = '\0';
                mode = -1;
                continue;
            }
            if(input == 127){
                if(max_command > 0){
                    max_command--;
                    *(command + max_command) = '\0';
                }
            }else{
                *(command + max_command) = input;
                *(command + max_command + 1) = '\0';
                max_command++;
            }
        }else{
            if(input == 27){
                mode = -1;
                continue;
            }else if(input == 127){
                if(max_data_pos > 0){
                    int tmp = *(data + max_data_pos - 1);
                    if(max_data_pos < max_data){
                        for(int i = max_data_pos - 1; i < max_data; i++){
                            *(data + i) = *(data + i + 1);
                        }
                    }
                    max_data_pos--;
                    max_data--;
                    pos_char--;
                    data_pos_char--;
                    *(line_info[1] + pos_line) -= 1;
                    *(line_info[0] + data_pos_line) -= 1;
                    if((pos_char < 3)){
                        *(line_info[1] + pos_line) += 1;
                        *(line_info[1] + pos_line - 1) += *(line_info[1] + pos_line) - 3;
                        for(int i = pos_line; i < line_counter; i++){
                            *(line_info[1] + i) = *(line_info[1] + i + 1);
                        }
                        *(line_info[1] + line_counter) = 3;

                        pos_line--;
                        line_counter--;
                        if(tmp == 10){
                            *(line_info[0] + data_pos_line) += 1;
                            *(line_info[0] + data_pos_line - 1) += *(line_info[0] + data_pos_line);
                            for(int i = data_pos_line; i < data_line_counter; i++){
                                *(line_info[0] + i) = *(line_info[0] + i + 1);
                            }
                            *(line_info[0] + data_line_counter) = 0;
                            data_line_counter--;
                            data_pos_line--;
                        }
                        pos_char = *(line_info[1] + pos_line);
                        data_pos_char = *(line_info[0] + data_pos_line);
                    }
                }
            }else{
                if(max_data_pos < max_data){
                    for(int i = max_data+1; i > max_data_pos; i--){
                        *(data + i) = *(data + i - 1);
                    }
                }
                *(data + max_data_pos) = input;
                max_data++;
                max_data_pos++;
                pos_char++;
                data_pos_char++;
                *(line_info[1] + pos_line) += 1;
                *(line_info[0] + data_pos_line) += 1;
                if(input == 10){
                    *(line_info[1] + pos_line) -= 1;
                    *(line_info[0] + data_pos_line) -= 1;
                    for(int i = data_line_counter; i > data_pos_line; i--){
                        *(line_info[0] + i + 1) = *(line_info[0] + i);
                    }
                    *(line_info[0] + data_pos_line + 1) = *(line_info[0] + data_pos_line) + 1 - data_pos_char;
                    *(line_info[0] + data_pos_line) = data_pos_char - 1;
                    for(int i = line_counter; i > pos_line; i--){
                        *(line_info[1] + i + 1) = *(line_info[1] + i);
                    }
                    *(line_info[1] + pos_line + 1) = *(line_info[1] + pos_line) + 1 - pos_char + 3;
                    *(line_info[1] + pos_line) = pos_char - 1;
                    data_line_counter++;
                    line_counter++;
                    // int add = 0;
                    // for(int i = 0; i < data_line_counter - 1; i++){
                    //     add += *(line_info[0] + i) + 1;
                    // }
                    // *(line_info[0] + data_line_counter - 1) = strlen(data + add) -1;
                    // *(line_info[1] + line_counter - 1) = strlen(data + add) -1;
                    data_pos_line++;
                    data_pos_char = 0;
                    pos_line++;
                    pos_char = 3;
                    //if(max_data_pos < max_data){
                    //}
                    // if(view_line_to - pos_line < 4){

                    // }
                }else if(pos_char > editor_width){
                    *(line_info[1] + pos_line) -= 1;
                    for(int i = line_counter; i > pos_line; i--){
                        *(line_info[1] + i + 1) = *(line_info[1] + i);
                    }
                    *(line_info[1] + pos_line + 1) = *(line_info[1] + pos_line) + 1 - pos_char + 3;
                    *(line_info[1] + pos_line) = pos_char - 1;

                    line_counter++;
                    // int add = 0;
                    // for(int i = 0; i < data_line_counter - 2; i++){
                    //     add +=*(line_info[0] + i) + 1;
                    // }
                    // *(line_info[1] + line_counter - 1) = strlen(data + add) -1;

                    pos_line++;
                    pos_char = 3;
                }
            }
        }

        //usleep(100000);
    }
}
