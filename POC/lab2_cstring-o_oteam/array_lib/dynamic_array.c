#include "dynamic_array.h"

int dynamic_array_create(dynamic_array *array, size_t size) {
    if (!array) {
        return NULL_PTR_ERR;
    }

    if ((int) size < 0) {
        return BUFF_SIZE_ERR;
    }

    array->my_str_arr = malloc(sizeof(my_str_t*) * size);
    if (!array->my_str_arr) {
        return MEMORY_ALLOCATION_ERR;
    }
    array->size = 0;
    array->capacity = size;

    return 0;
}

int dynamic_array_free(dynamic_array *array, int free_elements) {
    if (!array) {
        return 0;
    }

    if (free_elements) {
        for (size_t i = 0; i < array->size; i++) {
            // Free both data inside string and string structure itself
            my_str_free(array->my_str_arr[i]);
            free(array->my_str_arr[i]);
        }
    }

    free(array->my_str_arr);
    array->my_str_arr = NULL;
    array->size = 0;
    array->capacity = 0;


    return 0;
}

int dynamic_array_append(dynamic_array *array, my_str_t *str) {
    if (!array || !str) {
        return NULL_PTR_ERR;
    }

    if (array->size+1 > array->capacity) {
        if (dynamic_array_reserve(array, array->capacity * INC_PARAM) == MEMORY_ALLOCATION_ERR) {
            return MEMORY_ALLOCATION_ERR;
        }
    }

    array->my_str_arr[array->size] = str;
    array->size += 1;

    return 0;
}

int dynamic_array_pop(dynamic_array *array, int free_element) {
    if (!array) {
        return NULL_PTR_ERR;
    }

    if (array->size > 0) {
        // Free both data inside string and string structure itself
        if (free_element) {
            my_str_free(array->my_str_arr[array->size]);
            free(array->my_str_arr[array->size]);
        }

        array->size -= 1;
    }

    return 0;
}

int dynamic_array_reserve(dynamic_array *array, size_t size) {
    if (!array) {
        return NULL_PTR_ERR;
    }

    if (size > array->capacity) {
        my_str_t **new_my_str_arr = malloc(size * sizeof(my_str_t*));
        if (!new_my_str_arr) {
            return MEMORY_ALLOCATION_ERR;
        }

        memcpy(new_my_str_arr, array->my_str_arr, array->size * sizeof(my_str_t*));
        free(array->my_str_arr);
        array->my_str_arr = new_my_str_arr;
        array->capacity = size;
    }

    return 0;
}
