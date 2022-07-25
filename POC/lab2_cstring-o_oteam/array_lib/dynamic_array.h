#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "../c_str_lib/c_string.h"

#define INC_PARAM 2

typedef struct {
    my_str_t **my_str_arr;
    size_t size;
    size_t capacity;
} dynamic_array;

/* Create dynamic array.
 * Return:
 * 0 if OK
 * NULL_PTR_ERROR if array is NULL
 * BUFF_SIZE_ERROR if size is invalid
 * MEMORY_ALLOCATION_ERROR if there was an error during memory allocation
*/
int dynamic_array_create(dynamic_array *array, size_t size);

/* Free dynamic array.
 * Return:
 * 0 always
*/
int dynamic_array_free(dynamic_array *array, int free_elements);

/* Append my_str_t to dynamic array.
 * Return:
 * 0 if OK
 * NULL_PTR_ERROR if array is NULL
 * MEMORY_ALLOCATION_ERROR if there was an error during memory allocation
*/
int dynamic_array_append(dynamic_array *array, my_str_t *str);

/* Remove last element from dynamic array.
 * Return:
 * 0 if OK
 * NULL_PTR_ERROR if array or str is NULL
*/
int dynamic_array_pop(dynamic_array *array, int free_element);

/* Reallocate memory for dynamic array.
 * Return:
 * 0 if OK
 * MEMORY_ALLOCATION_ERROR if there was an error during memory allocation
*/
int dynamic_array_reserve(dynamic_array *array, size_t size);

#endif //DYNAMIC_ARRAY_H
