// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "../c_str_lib/c_string.h"
#include "../array_lib/dynamic_array.h"
#include <ctype.h>

int loop_for_hex(my_str_t* str, my_str_t* to_str);
int hex(int cur_char, char* buf);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "ERROR: No input or output file was provided\n");
        return 1;
    }

    char *input_file_path = argv[1];
    char *output_file_path = argv[2];
    my_str_t str_from_file = {0};
    dynamic_array array = {0};
    my_str_create(&str_from_file, 100);
    dynamic_array_create(&array, 2);

    FILE *input_file = fopen(input_file_path, "r");
    if (my_str_read_file(&str_from_file, input_file) == IO_READ_ERR) {
        fprintf(stderr, "ERROR: Couldn't read %s\n", input_file_path);
        return 2;
    }
    fclose(input_file);

    size_t start = 0;
    size_t end;

    // Add words to dynamic array
    while (1) {
        // Find index of the closest space, tab or new line symbol
        end = my_str_find_if(&str_from_file, start, isspace);
        size_t prev_end = end;

        // If no spacing was found than the last word has been reached
        if (end == -1) {
            end = str_from_file.size_m;
        }

        char *c_str = malloc(end-start+1);
        c_str[end-start] = '\0';
        my_str_substr_cstr(&str_from_file, c_str, start, end);
        my_str_t *word = malloc(sizeof(my_str_t));
        my_str_create(word, end-start);
        my_str_from_cstr(word, c_str, end-start);
        free(c_str);
        dynamic_array_append(&array, word);

        // If last word has been reached the loop should stop
        if (prev_end == -1) {
            break;
        }
        start = end+1;
    }

    my_str_t str_final = {0};
    my_str_create(&str_final, 100);
    for (int i = 0; i < array.size; i++) {
        loop_for_hex(array.my_str_arr[i], &str_final);
    }

    FILE *output_file = fopen(output_file_path, "w");
    if (my_str_write_file(&str_final, output_file) == IO_WRITE_ERR) {
        fprintf(stderr, "ERROR: Couldn't write to %s\n", output_file_path);
        return 3;
    }
    fclose(output_file);

    my_str_free(&str_final);
    my_str_free(&str_from_file);
    dynamic_array_free(&array, 1);

    return 0;
}

int hex(int cur_char, char* buf){
    // array of hex symbols
    char data[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    // convert char to hex and put it in the buffer
    buf[1] = data[cur_char%16];
    buf[0] = data[cur_char/16];
    return 0;
}

int loop_for_hex(my_str_t* str, my_str_t* to_str) {
    for(int i = 0; i < str->size_m; ++i){
        char buf[3];
        int cur = my_str_getc(str, i);
        hex(cur, buf);
        buf[2] = '\0';
        my_str_append_cstr(to_str, buf);
        if(i != str->size_m-1) {
            my_str_append_cstr(to_str, " ");
        }
    }
    my_str_append_cstr(to_str, "\n");
    return 0;
}
