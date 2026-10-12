#include "axin_string_view.h"
#include "axin_test.h"
#include <stdbool.h>

static bool test_from_parts(void) {
    const char text[] = "Hello World!";
    axin_string_view_t sv = axin_strv_from_parts(text, 5);
    axin_test_assert(sv.data == text);
    axin_test_assert(sv.size == 5);
    return true;
}

static bool test_compare_equal(void) {
    axin_test_assert(axin_strv_compare(
        axin_strv_from_cstr("abc"),
        axin_strv_from_cstr("abc")) == 0);
    return true;
}

static bool test_compare_empty(void) {
    axin_test_assert(axin_strv_compare(
        axin_strv_from_cstr(""),
        axin_strv_from_cstr("")) == 0);
    axin_test_assert(axin_strv_compare(
        axin_strv_from_cstr(""),
        axin_strv_from_cstr("abc")) < 0);
    axin_test_assert(axin_strv_compare(
        axin_strv_from_cstr("abc"),
        axin_strv_from_cstr("")) > 0);
    return true;
}

static bool test_compare_prefix(void) {
    axin_test_assert(axin_strv_compare(
        axin_strv_from_cstr("abc"),
        axin_strv_from_cstr("abcd")) < 0);
    axin_test_assert(axin_strv_compare(
        axin_strv_from_cstr("abcd"),
        axin_strv_from_cstr("abc")) > 0);
    return true;
}

static bool test_compare_firstdiff(void) {
    axin_test_assert(axin_strv_compare(
        axin_strv_from_cstr("abz"),
        axin_strv_from_cstr("aca")) < 0);
    axin_test_assert(axin_strv_compare(
        axin_strv_from_cstr("aca"),
        axin_strv_from_cstr("abz")) > 0);
    axin_test_assert(axin_strv_compare(
        axin_strv_from_cstr("z"),
        axin_strv_from_cstr("aca")) > 0);
    axin_test_assert(axin_strv_compare(
        axin_strv_from_cstr("abz"),
        axin_strv_from_cstr("z")) < 0);
    return true;
}

static bool test_compare_with_signed_char(void) {
    const unsigned char a[] = { 0x80 };
    const unsigned char b[] = { 0x7F };
    axin_string_view_t high = axin_strv_from_parts(
        (const char*)a, 1);
    axin_string_view_t low = axin_strv_from_parts(
        (const char*)b, 1);
    axin_test_assert(axin_strv_compare(high, low) > 0);
    axin_test_assert(axin_strv_compare(low, high) < 0);
    return true;
}

static bool test_starts_with_match(void) {
    axin_test_assert(axin_strv_starts_with(
        axin_strv_from_cstr("abcdef"),
        axin_strv_from_cstr("abc")));
    axin_test_assert(axin_strv_starts_with(
        axin_strv_from_cstr("abc"),
        axin_strv_from_cstr("abc")));
    return true;
}

static bool test_starts_with_empty(void) {
    axin_test_assert(axin_strv_starts_with(
        axin_strv_from_cstr("abc"),
        axin_strv_from_cstr("")));
    axin_test_assert(axin_strv_starts_with(
        axin_strv_from_cstr("abc"),
        AXIN_STRV_EMPTY));
    axin_test_assert(!axin_strv_starts_with(
        axin_strv_from_cstr(""),
        axin_strv_from_cstr("a")));
    axin_test_assert(!axin_strv_starts_with(
        AXIN_STRV_EMPTY,
        axin_strv_from_cstr("a")));
    return true;
}

static bool test_starts_with_longer_prefix(void) {
    axin_test_assert(!axin_strv_starts_with(
        axin_strv_from_cstr("ab"),
        axin_strv_from_cstr("abc")));
    return true;
}

static bool test_starts_with_mismatch(void) {
    axin_test_assert(!axin_strv_starts_with(
        axin_strv_from_cstr("abc"),
        axin_strv_from_cstr("xbc")));
    axin_test_assert(!axin_strv_starts_with(
        axin_strv_from_cstr("abc"),
        axin_strv_from_cstr("ax")));
    axin_test_assert(!axin_strv_starts_with(
        axin_strv_from_cstr("abc"),
        axin_strv_from_cstr("bc")));
    return true;
}

static bool test_respects_size(void) {
    axin_test_assert(axin_strv_equal(
        axin_strv_from_parts("abc", 2),
        axin_strv_from_parts("abx", 2)));
    axin_test_assert(axin_strv_compare(
        axin_strv_from_parts("abc", 3),
        axin_strv_from_parts("abc", 2)) > 0);
    axin_test_assert(axin_strv_starts_with(
        axin_strv_from_parts("abc", 3),
        axin_strv_from_parts("abx", 2)));
    axin_test_assert(axin_strv_ends_with(
        axin_strv_from_parts("abc", 3),
        axin_strv_from_parts("bcx", 2)));
    return true;
}

static bool test_without_terminator(void) {
    axin_test_assert(axin_strv_equal(
        axin_strv_from_parts((const char[]){ 'a', 'b', 'c' }, 3),
        axin_strv_from_cstr("abc")));
    axin_test_assert(axin_strv_compare(
        axin_strv_from_parts((const char[]){ 'a', 'b', 'c' }, 3),
        axin_strv_from_cstr("abc")) == 0);
    axin_test_assert(axin_strv_starts_with(
        axin_strv_from_parts((const char[]){ 'a', 'b', 'c' }, 3),
        axin_strv_from_parts((const char[]){ 'a', 'b' }, 2)));
    axin_test_assert(axin_strv_ends_with(
        axin_strv_from_parts((const char[]){ 'a', 'b', 'c' }, 3),
        axin_strv_from_parts((const char[]){ 'b', 'c' }, 2)));
    return true;
}

static bool test_embedded_nul(void) {
    axin_test_assert(!axin_strv_equal(
        axin_strv_from_parts("a\0b", 3),
        axin_strv_from_parts("a\0c", 3)));
    axin_test_assert(axin_strv_compare(
        axin_strv_from_parts("a\0b", 3),
        axin_strv_from_parts("a\0c", 3)) < 0);
    axin_test_assert(!axin_strv_starts_with(
        axin_strv_from_parts("a\0b", 3),
        axin_strv_from_parts("a\0c", 3)));
    axin_test_assert(!axin_strv_ends_with(
        axin_strv_from_parts("a\0b", 3),
        axin_strv_from_parts("a\0c", 3)));
    return true;
}

static bool test_ends_with_match(void) {
    axin_test_assert(axin_strv_ends_with(
        axin_strv_from_cstr("abc"),
        axin_strv_from_cstr("bc")));
    axin_test_assert(axin_strv_ends_with(
        axin_strv_from_cstr("abc"),
        axin_strv_from_cstr("abc")));
    return true;
}

static bool test_ends_with_empty(void) {
    axin_test_assert(axin_strv_ends_with(
        axin_strv_from_cstr("abc"),
        axin_strv_from_cstr("")));
    axin_test_assert(axin_strv_ends_with(
        axin_strv_from_cstr("abc"),
        AXIN_STRV_EMPTY));
    axin_test_assert(!axin_strv_ends_with(
        axin_strv_from_cstr(""),
        axin_strv_from_cstr("abc")));
    axin_test_assert(!axin_strv_ends_with(
        AXIN_STRV_EMPTY,
        axin_strv_from_cstr("abc")));
    return true;
}

static bool test_ends_with_longer_suffix(void) {
    axin_test_assert(!axin_strv_ends_with(
        axin_strv_from_cstr("abc"),
        axin_strv_from_cstr("abcd")));
    return true;
}

static bool test_ends_with_mismatch(void) {
    axin_test_assert(!axin_strv_ends_with(
        axin_strv_from_cstr("abc"),
        axin_strv_from_cstr("bx")));
    axin_test_assert(!axin_strv_ends_with(
        axin_strv_from_cstr("abc"),
        axin_strv_from_cstr("xc")));
    axin_test_assert(!axin_strv_ends_with(
        axin_strv_from_cstr("abc"),
        axin_strv_from_cstr("ab")));
    return true;
}

static bool test_find_substr(void) {
    axin_test_assert(axin_strv_find(
        axin_strv_from_cstr("abcde"),
        axin_strv_from_cstr("bcd"), 0) == 1);
    axin_test_assert(axin_strv_find(
        axin_strv_from_cstr("abcde"),
        axin_strv_from_cstr("cde"), 0) == 2);
    return true;
}

static bool test_find_empty(void) {
    axin_test_assert(axin_strv_find(
        axin_strv_from_cstr("abc"),
        AXIN_STRV_EMPTY,
        1) == 1);
    axin_test_assert(axin_strv_find(
        AXIN_STRV_EMPTY,
        AXIN_STRV_EMPTY,
        0) == 0);
    return true;
}

int main() {
    const axin_test_case_t cases[] = {
        AXIN_TEST_CASE(test_from_parts),

        AXIN_TEST_CASE(test_compare_equal),
        AXIN_TEST_CASE(test_compare_empty),
        AXIN_TEST_CASE(test_compare_prefix),
        AXIN_TEST_CASE(test_compare_firstdiff),
        AXIN_TEST_CASE(test_compare_with_signed_char),

        AXIN_TEST_CASE(test_starts_with_match),
        AXIN_TEST_CASE(test_starts_with_empty),
        AXIN_TEST_CASE(test_starts_with_longer_prefix),
        AXIN_TEST_CASE(test_starts_with_mismatch),

        AXIN_TEST_CASE(test_ends_with_match),
        AXIN_TEST_CASE(test_ends_with_empty),
        AXIN_TEST_CASE(test_ends_with_longer_suffix),
        AXIN_TEST_CASE(test_ends_with_mismatch),

        AXIN_TEST_CASE(test_find_substr),
        AXIN_TEST_CASE(test_find_empty),

        AXIN_TEST_CASE(test_respects_size),
        AXIN_TEST_CASE(test_without_terminator),
        AXIN_TEST_CASE(test_embedded_nul),
    };
    return axin_test_run(cases, sizeof(cases) / sizeof(cases[0]));
}
