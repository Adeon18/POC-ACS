#pragma once
#ifndef C_STRINGS_H
#define C_STRINGS_H

#include <stdlib.h>
#include <stdint.h> // For size_t
#include <stdio.h> // for files and prinrin
#include <string.h> // for memset function
#include <errno.h>

#define NOT_FOUND_CODE (-1)
#define MEMORY_ALLOCATION_ERR (-2)
#define RANGE_ERR (-3)
#define IO_READ_ERR (-4)
#define IO_WRITE_ERR (-5)
#define NULL_PTR_ERR (-8)
#define BUFF_SIZE_ERR (-9)

#define ARR_LEN(a) sizeof(a)/sizeof(a[0])
#define BUF_SIZE 4096
#define FORMAT_SIZE 32

typedef struct {
    size_t capacity_m; // Block size
    size_t size_m;     // Actual size of the string
    char *data;       // Pointer on data block
} my_str_t;

/*
 * Creates empty dynamic string (my_str_t)
 * !important! user should always use my_str_create before using ANY other function
 * str: pointer to my_str_t struct that user wants to create
 * buf_size: size of mem (in bytes) that user wants to allocate for buffer
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str points to NULL
 *      MEMORY_ALLOCATION_ERR if there was an error when allocating memory for buffer
 */
int my_str_create(my_str_t* str, size_t buf_size);

/*
 * frees all data from given my_str_t structure
 * return:
 *     0 always
 */
int my_str_free(my_str_t* str);

/*
 * makes content of given my_str-string the same as given c-string.
 * cstr: c-string from which data should be taken
 * buf_size: new size of buffer for given; if buf_size == 0 then new buffer size = length of c-string
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or cstr is NULL
 *      BUFF_SIZE_ERR if buf_size if incorrect (too small)
 *      MEMORY_ALLOCATION_ERR if there occurred error while mem allocation
 */
int my_str_from_cstr(my_str_t* str, const char* cstr, size_t buf_size);

/*
 * returns actual size of my_str-string
 * if str == NULL than size = 0
 */
size_t my_str_size(const my_str_t* str);

/*
 * returns size of allocated memory - 1 for my_str-cstring
 * if str == NULL than capacity = 0
 */
size_t my_str_capacity(const my_str_t* str);

/*
 * returns 1 if string is empty
 * if str == NULL than it is empty
 */
int my_str_empty(const my_str_t* str);

/*
 * returns symbol on given index in my_str-string
 * return:
 *      NULL_PTR_ERR if str == NULL
 *      RANGE_ERR if index is bad
 */
int my_str_getc(const my_str_t* str, size_t index);

/*
 * puts given symbol on given position in my_str-string
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or c is NULL
 *      RANGE_ERR if index is bad
 */
int my_str_putc(my_str_t* str, size_t index, char c);

/*
 * returns c-string with the same content as my_str-string
 * result c-string is constant and can be incorrect after changing my_str content
 * returns NULL if str is NULL
 */
const char* my_str_get_cstr(my_str_t* str);

/*
 * copies content of one my_str-string to other
 * after manipulations with second string, its buffer will be like capacity of first
 * if reserve == 1 else if reserve == 0 size of first
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if one of string or both are NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_copy(const my_str_t* from,  my_str_t* to, int reserve);

/*
 *  clears content of string, does nothing if string is NULL
 *  return:
 *      always 0
 */
int my_str_clear(my_str_t* str);

/*
 * inserts given char in the given position in my_str-cstring
 * if needed increases buffer
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or c is NULL
 *      RANGE_ERR if position is bad
 *      MEMORY_ALLOCATION_ERR if occurred error during memory allocation
 */
int my_str_insert_c(my_str_t* str, char c, size_t pos);

/*
 * inserts given my_str-string into another one at given position
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or from is NULL
 *      RANGE_ERR if position is bad
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_insert(my_str_t* str, const my_str_t* from, size_t pos);

/*
 * inserts given c-string into given my_str-string\
 * return:
 *      the same as in my_str_insert(...) function
 */
int my_str_insert_cstr(my_str_t* str, const char* from, size_t pos);

/*
 * appends one my_str-string to another
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if from ot str is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_append(my_str_t* str, const my_str_t* from);


/*
 * appends c-string to my_str-string
 * return: the same as in my_str_append(...) function
 */
int my_str_append_cstr(my_str_t* str, const char* from);

/*
 * pushes given char on the end of my_str-string
 * if needed increases buffer by INC_PARAM (2)
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or c is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during allocating memory for buffer
 */
int my_str_append_c(my_str_t* str, char c);

/*
 * saves substring of my_str-string in given bounds to given my_str-string
 * return:
 *      0  if oK
 *      NULL_PTR_ERR if from or to is NULL
 *      RANGE_ERR if boundaries are bad
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_substr(const my_str_t* from, my_str_t* to, size_t beg, size_t end);

/*
 * saves my_str-string substring in given bounds to given c-string
 * !important! given c-string should be big enough
 * return:
 *      0  if oK
 *      NULL_PTR_ERR if from or to is NULL
 *      RANGE_ERR if boundaries are bad
 */
int my_str_substr_cstr(const my_str_t* from, char* to, size_t beg, size_t end);

/*
 * return symbols with indexes [beg, end), shifts reminder to the left
 * end > size is not an error - erase all possible
 * beg > size is an error
 * return:
 *      0 if OK
 *      RANGE_ERR if boundaries are bad
 *      MEMORY_ALLOCATION_ERROR if there was an error during memory allocation
 *      NULL_PTR_ERR if from is NULL
 */
int my_str_erase(my_str_t* str, size_t beg, size_t end);

/*
 * pops last symbol in the my_str-string
 * return:
 *      0  if OK
 *      RANGE_ERR if size of string is already 0
 *      NULL_PTR_ERR if str is NULL
 *      else - popped symbol
 */
int my_str_popback(my_str_t* str);

/*
 * increases given my_str-string's buffer to given value.
 * if buf_size < my_str-string capacity, does nothing
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_reserve(my_str_t* str, size_t buf_size);

/*
 * decreases buffer of given my_str-string to size of string
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if from ot str is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_shrink_to_fit(my_str_t* str);

/*
 * resizes my_str-string to given size.
 * if given size > size of string than filles next bytes with given chars
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_resize(my_str_t* str, size_t new_size, char sym);

/* ????????????????????????????????????????????
 * pushes given char on the end of my_str-string
 * if needed increases buffer by INC_PARAM (2)
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or c is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during allocating memory for buffer
 */

/* ???????????????????????????????????????????? */
size_t my_str_find(const my_str_t* str, const my_str_t* tofind, size_t from);

/*
 * compares two my_str-strings like conventional c-strings (in lexicographical order)
 * return:
 *      0 if str1 == str2
 *      -1 if str1 < str2
 *      1  if str1 > str2
 *      NULL_PTR_ERR if str1 or str2 is NULL
 */
int my_str_cmp(const my_str_t* str1, const my_str_t* str2);

/*
 * compares my_str-strings and c-stirng lexicographical order
 * return: the same as in my_str_cmp
 */
int my_str_cmp_cstr(const my_str_t* str1, const char* cstr2);

/*
 * returns position of given symbol in my_str-string, -1 if it's not there
 * starts search from given position
 * return:
 *      position of char symbol if its in string
 *      NOT_FOUND_CODE if it's not in string
 *      NULL_PTR_ERR if str is NULL
 */
int my_str_find_c(const my_str_t* str, char tofind, size_t from);

/*
 * returns position of symbol that predicate on ot returns 1, if there is no symbol like that than -1
 * return:
 *      position of char symbol that satisfies predicate
 *      NOT_FOUND_CODE if it's not in string
 *      NULL_PTR_ERR if str or predicate is NULL
 */
int my_str_find_if(const my_str_t* str, size_t beg, int (*predicat)(int));

/*
 * reads file and saves it's content into given my_str-string
 * resizes string's buffer if needed
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or file is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_read_file(my_str_t* str, FILE* file);

/*
 * reads content from stdin and saves to my_str-string
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during increasing buffer
 *      IO_READ_ERR if there was error while reading
 */
int my_str_read(my_str_t* str);

/*
 * reads file and saves it's content into my_str-string, stops when reached delimiter or EOF
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or file is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_read_file_delim(my_str_t* str, FILE* file, char delimiter);

/*
 * writes content of given my_str-string to given file
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or file is NULL
 *      IO_WRITE_ERR if error occurred while writing
 */
int my_str_write_file(const my_str_t* str, FILE* file);

/*
 * writes content of given my_str-string into console
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str is NULL
 *      IO_WRITE_ERR if error occurred while writing
 */
int my_str_write(const my_str_t* str);

static size_t length_cstr(const char * str);

#endif // C_STRINGS_H

