#ifndef AXIN_CLIB_STRING_VIEW_H
#define AXIN_CLIB_STRING_VIEW_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    const char* data;
    size_t size;
} axin_string_view_t;

static const axin_string_view_t AXIN_STRV_EMPTY = {NULL, 0};

static inline axin_string_view_t axin_strv_from_parts(
    const char* data,
    size_t size
) {
    return (axin_string_view_t){data, size};
}

static inline axin_string_view_t axin_strv_from_cstr(
    const char* cstr
) {
    size_t size = 0;
    for (int i = 0; cstr[i] != '\0'; i++) size++;
    return (axin_string_view_t){cstr, size};
}

static inline bool axin_strv_empty(
    axin_string_view_t sv
) {
    return sv.data == NULL && sv.size == 0;
}

static inline const char* axin_strv_at(
    axin_string_view_t sv,
    size_t index
) {
    if (index >= sv.size) return NULL;
    return sv.data + index;
}

static inline axin_string_view_t axin_strv_substr(
    axin_string_view_t src,
    size_t pos,
    size_t count
) {
    if (pos >= src.size) return AXIN_STRV_EMPTY;
    axin_string_view_t r = { .data = src.data + pos};
    if (count >= src.size - pos) r.size = src.size - pos;
    else r.size = count;
    return r;
}

static inline axin_string_view_t axin_strv_prefix(
    axin_string_view_t sv,
    size_t count
) {
    if (count >= sv.size) return sv;
    return (axin_string_view_t){ sv.data, count };
}

static inline axin_string_view_t axin_strv_suffix(
    axin_string_view_t sv,
    size_t count
) {
    if (count >= sv.size) return sv;
    return (axin_string_view_t){ sv.data + sv.size - count, count };
}

static inline bool axin_strv_equal(
    axin_string_view_t a,
    axin_string_view_t b
) {
    if (a.size != b.size) return false;
    for (int i = 0; i < a.size; i++) {
        if (a.data[i] != b.data[i]) return false;
    }
    return true;
}

#endif // AXIN_CLIB_STRING_VIEW_H
