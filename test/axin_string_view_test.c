#include "axin_string_view.h"

#include <stdio.h>

int main() {
    const char* str = "Hello World!";
    printf("axin_string_view test:\n");
    printf("from_parts: ");
    axin_string_view_t sv_from_parts = axin_strv_from_parts(str, 5);
    if (axin_strv_equal(
        sv_from_parts,
        axin_strv_from_cstr("Hello")
    )) {
        printf("pass\n");
    } else {
        printf("not pass. result: ");
        axin_strv_print(sv_from_parts);
        return 1;
    }
    return 0;
}
