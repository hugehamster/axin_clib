// axin's dynamic array

#ifndef AXIN_CLIB_ARRAY_H
#define AXIN_CLIB_ARRAY_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    unsigned char* data;
    size_t size;
    size_t capacity;
    size_t element_size;
} axin_array_t;

static inline void axin_array_init(
    axin_array_t* array,
    size_t element_size
) {
    if (element_size <= 0) return;
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
    array->element_size = element_size;
}

static inline void axin_array_destroy(
    axin_array_t* array
) {
    free(array->data);
    array->data = NULL;
}

static inline size_t axin_array_size(
    const axin_array_t* array
) {
    return array->size;
}

static inline bool axin_array_empty(
    const axin_array_t* array
) {
    return array->size == 0;
}

#define axin_array_at_unsafe(array, index) \
    ((array)->data + (index) * (array)->element_size)

static inline void* axin_array_at(
    const axin_array_t* array,
    size_t index
) {
    if (index >= array->size) return NULL;
    return axin_array_at_unsafe(array, index);
}

static inline void* axin_array_front(
    const axin_array_t* array
) {
    if (axin_array_empty(array)) return NULL;
    return axin_array_at(array, 0);
}

static inline void* axin_array_back(
    const axin_array_t* array
) {
    if (axin_array_empty(array)) return NULL;
    return axin_array_at(array, array->size - 1);
}

static inline void axin_array_clear(
    axin_array_t* array
) {
    array->size = 0;
}

static inline size_t axin_array_reserve(
    axin_array_t* array,
    size_t capacity
) {
    if (array->capacity >= capacity) return array->capacity;
    if (capacity >= SIZE_MAX / array->element_size) return array->capacity;
    size_t byte_size = capacity * array->element_size;
    unsigned char* tmp = realloc(array->data, byte_size);
    if (tmp == NULL) return array->capacity;
    array->data = tmp;
    array->capacity = capacity;
    return capacity;
}

static inline size_t axin_array_shrink_to_fit(
    axin_array_t* array
) {
    if (array->capacity == array->size) return array->capacity;
    if (array->size == 0) {
        free(array->data);
        array->data = NULL;
        array->size = 0;
        array->capacity = 0;
        return 0;
    }
    size_t byte_size = array->size * array->element_size;
    void* tmp = realloc(array->data, byte_size);
    if (tmp == NULL) return array->capacity;
    array->data = tmp;
    array->capacity = array->size;
    return array->size;
}

static inline size_t axin_array_insert(
    axin_array_t* array,
    size_t index,
    const void* data
) {
    if (index > array->size) return SIZE_MAX;
    void* buf = malloc(array->element_size);
    if (buf == NULL) return SIZE_MAX;
    memcpy(buf, data, array->element_size);
    if (array->size == array->capacity) {
        size_t new_capacity = array->capacity == 0 ? 1 : array->capacity * 2;
        if (axin_array_reserve(array, new_capacity) != new_capacity) {
            free(buf);
            return SIZE_MAX;
        }
    }
    void* dst = axin_array_at_unsafe(array, index + 1);
    void* src = axin_array_at_unsafe(array, index);
    size_t size_to_move = (array->size - index) * array->element_size;
    memmove(dst, src, size_to_move);
    memcpy(src, buf, array->element_size);
    array->size++;
    free(buf);
    return index;
}

static inline size_t axin_array_push_front(
    axin_array_t* array,
    const void* data
) {
    return axin_array_insert(array, 0, data);
}

static inline size_t axin_array_push_back(
    axin_array_t* array,
    const void* data
) {
    return axin_array_insert(array, array->size, data);
}

static inline size_t axin_array_append_n(
    axin_array_t* array,
    const void* data,
    size_t count
) {
    if (count == 0) return array->size;
    if (count >= SIZE_MAX / array->element_size) return SIZE_MAX;
    void* buf = malloc(count * array->element_size);
    if (buf == NULL) return SIZE_MAX;
    memmove(buf, data, count * array->element_size);
    if (array->size + count > array->capacity) {
        size_t new_capacity = array->size + count;
        if (axin_array_reserve(array, new_capacity) != new_capacity) {
            free(buf);
            return SIZE_MAX;
        }
    }
    memcpy(axin_array_at_unsafe(array, array->size),
        buf, count * array->element_size);
    array->size += count;
    free(buf);
    return array->size;
}

static inline size_t axin_array_remove(
    axin_array_t* array,
    size_t index
) {
    if (index >= array->size) return SIZE_MAX;
    if (index == array->size - 1) {
        array->size--;
        return array->size;
    }
    void* src = axin_array_at_unsafe(array, index + 1);
    void* dst = axin_array_at_unsafe(array, index);
    size_t size_to_move = (array->size - index - 1) * array->element_size;
    memmove(dst, src, size_to_move);
    array->size--;
    return index;
}

static inline size_t axin_array_remove_range(
    axin_array_t* array,
    size_t index,
    size_t count
) {
    if (index >= array->size) return SIZE_MAX;
    if (count >= array->size - index) {
        array->size = index;
    } else {
        void* dst = axin_array_at(array, index);
        void* src = axin_array_at(array, index + count);
        size_t size_to_move = (array->size - index - count) * array->element_size;
        memmove(dst, src, size_to_move);
        array->size -= count;
    }
    return index;
}

static inline bool axin_array_pop_front(
    axin_array_t* array,
    void* out
) {
    if (axin_array_empty(array)) return false;
    memcpy(out, axin_array_at(array, 0), array->element_size);
    axin_array_remove(array, 0);
    return true;
}

static inline bool axin_array_pop_back(
    axin_array_t* array,
    void* out
) {
    if (axin_array_empty(array)) return false;
    memcpy(out, axin_array_at(array, array->size - 1), array->element_size);
    axin_array_remove(array, array->size - 1);
    return true;
}

#endif // AXIN_CLIB_ARRAY_H
