// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "c_string.h"

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
int my_str_create(my_str_t* str, size_t buf_size) {
    if (!str)
        return NULL_PTR_ERR;

    // overflow check
    buf_size = (buf_size == SIZE_MAX) ? buf_size : buf_size + 1;

    str->data = (char *) calloc(buf_size, sizeof(char));
    if (!str->data)
        return MEMORY_ALLOCATION_ERR;

    str->size_m = 0;
    str->capacity_m = buf_size-1;

    return 0;
}

/*
 * frees all data from given my_str_t structure
 * return:
 *     0 always
 */
int my_str_free(my_str_t* str) {
    if (!str)
        return 0;

    str->size_m = 0;
    str->capacity_m = 0;

    if (!str->data)
        return 0;

    free(str->data);
    str->data = NULL;

    return 0;
}

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
int my_str_from_cstr(my_str_t* str, const char* cstr, size_t buf_size) {
    if (!str || !cstr)
        return NULL_PTR_ERR;

    // determines size of string
    size_t length = length_cstr(cstr);
    if (buf_size < length && buf_size != 0)
        return BUFF_SIZE_ERR;

    int reserve, err;
    // determines whether buffer should be increased, as well as its size
    reserve = (buf_size == 0 || str->capacity_m < length) ? 1 : 0;
    buf_size = (!buf_size) ? length : buf_size;

    if (reserve) {
        err = my_str_free(str);
        if (err != 0) return err;

        // allocates memory equal to the buf size
        err = my_str_reserve(str, buf_size);
        if (err != 0) return err;
    }

    memcpy(str->data, cstr, length);
    str->size_m = length;

    return 0;
}

/*
 * returns actual size of my_str-string
 * if str == NULL than size = 0
 */
size_t my_str_size(const my_str_t* str) {
    return (!str) ? 0: str->size_m;
}

/*
 * returns size of allocated memory - 1 for my_str-cstring
 * if str == NULL than capacity = 0
 */
size_t my_str_capacity(const my_str_t* str) {
    return (!str) ? 0 : str->capacity_m;
}

/*
 * returns 1 if string is empty
 * if str == NULL than it is empty
 */
int my_str_empty(const my_str_t* str) {
    return (!str || !str->size_m) ? 1 : 0;
}

/*
 * returns symbol on given index in my_str-string
 * return:
 *      NULL_PTR_ERR if str == NULL
 *      RANGE_ERR if index is bad
 */
int my_str_getc(const my_str_t* str, size_t index) {
    if (!str) return NULL_PTR_ERR;

    if (index >= str->size_m)
        return RANGE_ERR;

    return str->data[index];
}

/*
 * puts given symbol on given position in my_str-string
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or c is NULL
 *      RANGE_ERR if index is bad
 */
int my_str_putc(my_str_t* str, size_t index, char c) {
    if (!str || !str->data || !c)
        return NULL_PTR_ERR;

    if (index >= str->size_m)
        return RANGE_ERR;

    str->data[index] = c;

    return 0;
}


/*
 * returns c-string with the same content as my_str-string
 * result c-string is constant and can be incorrect after changing my_str content
 * returns NULL if str is NULL
 */
const char* my_str_get_cstr(my_str_t* str) {
    if (!str || !str->data)
        return NULL;

    str->data[str->size_m] = '\0';
    return str->data;
}

/*
 * copies content of one my_str-string to other
 * after manipulations with second string, its buffer will be like capacity of first
 * if reserve == 1 else if reserve == 0 size of first
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if one of string or both are NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_copy(const my_str_t* from,  my_str_t* to, int reserve) {
    if (!from || !to)
        return NULL_PTR_ERR;

    int err;
    err = my_str_reserve(to, !reserve ? from->size_m : from->capacity_m);
    if (err != 0) return err;

    memcpy(to->data, from->data, from->size_m);
    to->size_m = from->size_m;

    return 0;
}

/*
 *  clears content of string, does nothing if string is NULL
 *  return:
 *      always 0
 */
int my_str_clear(my_str_t* str) {
    if (!str) return 0;

    memset(str->data, 0, str->capacity_m);
    str->size_m = 0;

    return 0;
}

/*
 * inserts given char in the given position in my_str-c string
 * if needed increases buffer
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or c is NULL
 *      RANGE_ERR if position is bad
 *      MEMORY_ALLOCATION_ERR if occurred error during memory allocation
 */
int my_str_insert_c(my_str_t* str, char c, size_t pos) {
    if (!str || !c)
        return NULL_PTR_ERR;

    if (pos > str->size_m)
        return RANGE_ERR;

    size_t i = my_str_size(str);

    int err;
    // we should increase capacity to have enough space for element to insert
    // as well as an additional space for end-of-string character
    if (str->size_m + 1 > str->capacity_m) {
        err = my_str_reserve(str, str->capacity_m + 2);
        if (err != 0) return err;
    }

    // append zero character that is used as placeholder
    err = my_str_append_c(str, 'i');
    if (err != 0) return err;

    for (; i > pos; --i)
        str->data[i] = str->data[i - 1];
    str->data[i] = c;

    return 0;
}

/*
 * inserts given my_str-string into another one at given position
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or from is NULL
 *      RANGE_ERR if position is bad
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_insert(my_str_t* str, const my_str_t* from, size_t pos){
    if(!str || !from)
        return NULL_PTR_ERR;

    if (pos > str->size_m)
        return RANGE_ERR;

    int err = my_str_insert_cstr(str, my_str_get_cstr((my_str_t *)from), pos);
    if (err != 0) return err;

    return 0;
}

/*
 * inserts given c-string into given my_str-string\
 * return:
 *      the same as in my_str_insert(...) function
 */
int my_str_insert_cstr(my_str_t* str, const char* from, size_t pos){
    if(!str || !from)
        return NULL_PTR_ERR;

    if (pos > str->size_m)
        return RANGE_ERR;

    size_t length_from = length_cstr(from);
    int err, i;
    err = my_str_reserve(str, str->capacity_m + length_from);
    if (err != 0) return err;

    for(i = str->size_m - 1; i >= (int)pos; i--)
        str->data[i + length_from] = str->data[i];
    memcpy(str->data + pos, from, length_from);
    str->size_m += length_from;

    return 0;
}

/*
 * appends one my_str-string to another
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if from ot str is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_append(my_str_t* str, const my_str_t* from){
    if (!str || !from)
        return NULL_PTR_ERR;

    int err = my_str_reserve(str, str->capacity_m + from->capacity_m);
    if (err != 0) return err;

    memcpy(str->data + str->size_m, from->data, from->size_m);
    str->size_m += from->size_m;

    return 0;
}

/*
 * appends c-string to my_str-string
 * return: the same as in my_str_append(...) function
 */
int my_str_append_cstr(my_str_t* str, const char* from){
    if (!str || !from)
        return NULL_PTR_ERR;

    size_t length_from = length_cstr(from);
    int err = my_str_reserve(str, str->capacity_m + length_from);
    if (err != 0)  return err;

    memcpy(str->data + str->size_m, from, length_from);
    str->size_m += length_from;
    return 0;
}

/*
 * pushes given char on the end of my_str-string
 * if needed increases buffer by INC_PARAM (2)
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or c is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during allocating memory for buffer
 */
int my_str_append_c(my_str_t* str, char c) {
    if (!str || !c)
        return NULL_PTR_ERR;

    if (str->capacity_m == str->size_m) {
        int err = my_str_reserve(str, str->capacity_m * 2);
        if (err != 0) return err;
    }

    str->data[str->size_m++] = c;

    return 0;
}

/*
 * saves substring of my_str-string in given bounds to given my_str-string
 * return:
 *      0  if oK
 *      NULL_PTR_ERR if from or to is NULL
 *      RANGE_ERR if boundaries are bad
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_substr(const my_str_t* from, my_str_t* to, size_t beg, size_t end) {
    if (!from || !to)
        return NULL_PTR_ERR;

    if (beg > end || beg > from->size_m)
        return RANGE_ERR;

    // so not to allocate much memory, let's bound the end index
    end = end < from->size_m ? end : from->size_m;

    int err = my_str_reserve(to, (end - beg) * 2 + 1);
    if (err != 0) return err;

    memcpy(to->data, from->data + beg, end - beg);
    to->size_m = end - beg;

    return 0;
}

/*
 * saves my_str-string substring in given bounds to given c-string
 * !important! given c-string should be big enough
 * return:
 *      0  if oK
 *      NULL_PTR_ERR if from or to is NULL
 *      RANGE_ERR if boundaries are bad
 */
int my_str_substr_cstr(const my_str_t* from, char* to, size_t beg, size_t end) {
    if (!from || !to)
        return NULL_PTR_ERR;

    if (beg > end || beg > from->size_m)
        return RANGE_ERR;

    memcpy(to,  (from->data + beg), end - beg);

    return 0;
}

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
int my_str_erase(my_str_t* str, size_t beg, size_t end){
    if(!str)
        return NULL_PTR_ERR;

    end = end < str->size_m ? end : str->size_m;

    if (beg > str->size_m || end < beg)
        return RANGE_ERR;

    size_t erase_seg = end - beg;
    memmove(str->data + beg, str->data + end, str->size_m - end);

    str->size_m -= erase_seg;
    int err = my_str_reserve(str, str->capacity_m - erase_seg);
    if (err != 0) return err;

    return 0;
}
/*
 * pops last symbol in the my_str-string
 * return:
 *      0  if OK
 *      RANGE_ERR if size of string is already 0
 *      NULL_PTR_ERR if str is NULL
 *      else - popped symbol ??????????????????????
 */
int my_str_popback(my_str_t* str) {
    if (!str)
        return NULL_PTR_ERR;

    if (str->size_m == 0)
        return RANGE_ERR;

    str->size_m--;
    char popped = str->data[str->size_m];
    str->data[str->size_m] = '\0';

    return popped;
}

/*
 * increases given my_str-string's buffer to given value.
 * if buf_size < my_str-string capacity, does nothing
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_reserve(my_str_t* str, size_t buf_size) {
    if (!str)
        return NULL_PTR_ERR;

    // does nothing if capacity is larger than buffer
    if (buf_size <= str->capacity_m)
        return 0;

    int err = 0;
    my_str_t larger_str = {0};
    err = my_str_create(&larger_str, buf_size);
    if (err != 0) return  err;

    memcpy(larger_str.data, str->data, str->size_m);
    larger_str.size_m = str->size_m;

    my_str_free(str);

    // redirects the pointer of newly created string to the str
    str->data = larger_str.data;
    str->size_m = larger_str.size_m;
    str->capacity_m = larger_str.capacity_m;

    return 0;
}

/*
 * decreases buffer of given my_str-string to size of string
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if from ot str is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_shrink_to_fit(my_str_t* str){
    if (!str)
        return NULL_PTR_ERR;

    my_str_t new_string = {0};
    int err = my_str_create(&new_string, str->size_m);
    if (err != 0) return err;

    err = my_str_copy(str, &new_string, 0);
    if (err != 0) return err;

    my_str_free(str);

    str->data = new_string.data;
    str->size_m = new_string.size_m;
    str->capacity_m = new_string.capacity_m;

    return 0;
}


/*
 * resizes my_str-string to given size.
 * if given size > size of string than filles next bytes with given chars
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_resize(my_str_t* str, size_t new_size, char sym){
    if (!str)
        return NULL_PTR_ERR;

    if (new_size > str->capacity_m){
        int err = my_str_reserve(str, 2 * new_size);
        if (err != 0) return err;
    }

    if (new_size > str->size_m)
        memset(str->data + str->size_m, (int) sym, new_size - str->size_m);
    str->size_m = new_size;

    return 0;
}

size_t my_str_find(const my_str_t* str, const my_str_t* tofind, size_t from) {
    if (!str || !tofind)
        return (size_t) NULL_PTR_ERR;

    size_t t_n = str->size_m;
    size_t p_n = tofind->size_m;
    for (size_t i = from; i < t_n; i++) {
        size_t j;
        for (j = 0; j < p_n; j++) {
            if (str->data[i + j] != tofind->data[j])
                break;
        }
        if (j == p_n && p_n != 0)
            return i;
    }

    return (size_t)NOT_FOUND_CODE;
}

/*
 * compares two my_str-strings like conventional c-strings (in lexicographical order)
 * return:
 *      0 if str1 == str2
 *      -1 if str1 < str2
 *      1  if str1 > str2
 *      NULL_PTR_ERR if str1 or str2 is NULL
 */
int my_str_cmp(const my_str_t* str1, const my_str_t* str2){
    if (!str1 || !str2)
        return NULL_PTR_ERR;

    return my_str_cmp_cstr(str1, my_str_get_cstr((my_str_t *) str2));
}

/*
 * compares my_str-strings and c-stirng lexicographical order
 * return: the same as in my_str_cmp
 */
int my_str_cmp_cstr(const my_str_t* str1, const char* cstr2){
    if (!str1 || !cstr2)
        return NULL_PTR_ERR;

    size_t i = 0;
    size_t second_length = length_cstr(cstr2);
    for (; i < str1->size_m && i < second_length; i++){
        if (str1->data[i] == cstr2[i])
            continue;
        return (str1->data[i] < cstr2[i]) ? -1 : 1;
    }

    // two string are equal
    if (i == str1->size_m && i == second_length)
        return 0;

    return (i == str1->size_m) ? -1 : 1;
}

/*
 * returns position of given symbol in my_str-string, -1 if it's not there
 * starts search from given position
 * return:
 *      position of char symbol if its in string
 *      NOT_FOUND_CODE if it's not in string
 *      NULL_PTR_ERR if str is NULL
 */
int my_str_find_c(const my_str_t* str, char tofind, size_t from) {
    if (!str || !tofind)
        return NULL_PTR_ERR;

    for (size_t i = from; i < str->size_m; i++)
        if (str->data[i] == tofind)
            return i;

    return NOT_FOUND_CODE;
}

/*
 * returns position of symbol that predicate on ot returns 1, if there is no symbol like that than -1
 * return:
 *      position of char symbol that satisfies predicate
 *      NOT_FOUND_CODE if it's not in string
 *      NULL_PTR_ERR if str or predicate is NULL
 */
int my_str_find_if(const my_str_t* str, size_t beg, int (*predicat)(int)) {
    if (!str || !predicat)
        return NULL_PTR_ERR;

    for (size_t i = beg; i < str->size_m; i++) {
//        printf("%c\n", str->data[i]);
        if (predicat((int) str->data[i]))
            return i;
    }

    return NOT_FOUND_CODE;
}

/*
 * reads file and saves it's content into given my_str-string
 * resizes string's buffer if needed
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or file is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_read_file(my_str_t* str, FILE* file) {
    if (!str || !file)
        return NULL_PTR_ERR;

    my_str_clear(str);

    int err;
    char buf[BUF_SIZE];

    while (fgets(buf, BUF_SIZE, file)) {
        err = my_str_append_cstr(str, buf);
        if (err != 0) return err; // handles Memory allocation error

        memset(buf, 0, BUF_SIZE);

        if (feof(file))
            break;
        if (ferror(file))
            return IO_READ_ERR;
    }

    return 0;
}

/*
 * reads content from stdin and saves to my_str-string
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during increasing buffer
 *      IO_READ_ERR if there was error while reading
 */
int my_str_read(my_str_t* str) {
    if (!str) return NULL_PTR_ERR;

    int err = my_str_read_file(str, stdin);
    if (err != 0) return err;

    // ugly, though better fixes require std functions for
    // working with c strings (deletes trailing whitespace)
    str->size_m = (str->data[str->size_m-1] == '\n') ? str->size_m - 1 : str->size_m;

    return 0;
}

/*
 * reads file and saves it's content into my_str-string, stops when reached delimiter or EOF
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or file is NULL
 *      MEMORY_ALLOCATION_ERR if there was an error during memory allocation
 */
int my_str_read_file_delim(my_str_t* str, FILE* file, char delimiter) {
    if (!str || !file)
        return NULL_PTR_ERR;

    my_str_clear(str);

    int err;
    char buf[BUF_SIZE];

    while (fgets(buf, BUF_SIZE, file)) {
        for (size_t i = 0; i < BUF_SIZE && buf[i] != '\0'; i++) {
            if (buf[i] == delimiter)
                return 0;

            err = my_str_append_c(str, buf[i]);
            if (err != 0) return err;
        }

        memset(buf, 0, BUF_SIZE);

        if (feof(file))
            break;
        if (ferror(file))
            return IO_READ_ERR;
    }

    return 0;

}

/*
 * writes content of given my_str-string to given file
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str or file is NULL
 *      IO_WRITE_ERR if error occurred while writing
 */
int my_str_write_file(const my_str_t* str, FILE* file) {
    if (!str || !file)
        return NULL_PTR_ERR;

    str->data[str->size_m] = '\0';
    int err = fputs(str->data, file);
    if (err == EOF) return IO_WRITE_ERR;

    return 0;
}

/*
 * writes content of given my_str-string into console
 * return:
 *      0  if OK
 *      NULL_PTR_ERR if str is NULL
 *      IO_WRITE_ERR if error occurred while writing
 */
int my_str_write(const my_str_t* str) {
    return my_str_write_file(str, stdout);
}

// function, which calculate length of c-string with assumption that str!=NULL
static size_t length_cstr(const char * str){
    int length= 0;
    for (; str[length] != '\0'; length++);

    return length;
}
