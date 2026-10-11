#ifndef AXIN_CLIB_STRING_VIEW_H
#define AXIN_CLIB_STRING_VIEW_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    const char* data;
    size_t size;
} axin_string_view_t;

static const axin_string_view_t AXIN_STRV_EMPTY = {NULL, 0};

static inline axin_string_view_t axin_strv_from_parts(
    const char* data,
    size_t size
) {
    return (axin_string_view_t){ data, size };
}

static inline axin_string_view_t axin_strv_from_cstr(
    const char* cstr
) {
    size_t size = strlen(cstr);
    return (axin_string_view_t){ cstr, size };
}

static inline axin_string_view_t axin_strv_from_range(
    const char* begin,
    const char* end
) {
    if (end < begin) return AXIN_STRV_EMPTY;
    return (axin_string_view_t){ begin, end - begin };
}

static inline void axin_strv_print(
    axin_string_view_t sv
) {
    for (size_t i = 0; i < sv.size; i++) {
        putc(sv.data[i], stdout);
    }
    fflush(stdout);
}

static inline bool axin_strv_empty(
    axin_string_view_t sv
) {
    return sv.size == 0;
}

static inline size_t axin_strv_size(
    axin_string_view_t sv
) {
    return sv.size;
}

static inline const char* axin_strv_at(
    axin_string_view_t sv,
    size_t index
) {
    if (index >= sv.size) return NULL;
    return sv.data + index;
}

static inline const char* axin_strv_at_unsafe(
    axin_string_view_t sv,
    size_t index
) {
    return sv.data + index;
}

static inline bool axin_strv_try_at(
    axin_string_view_t sv,
    size_t index,
    char* out
) {
    if (index >= sv.size) return false;
    *out = *axin_strv_at(sv, index);
    return true;
}

static inline char axin_strv_front(
    axin_string_view_t sv
) {
    if (axin_strv_empty(sv)) return 0;
    return *axin_strv_at(sv, 0);
}

static inline char axin_strv_back(
    axin_string_view_t sv
) {
    if (axin_strv_empty(sv)) return 0;
    return *axin_strv_at(sv, sv.size - 1);
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
    for (size_t i = 0; i < a.size; i++) {
        if (a.data[i] != b.data[i]) return false;
    }
    return true;
}

#endif // AXIN_CLIB_STRING_VIEW_H
