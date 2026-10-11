#include "axin_string_view.h"

#include <stdio.h>

int main() {
    const char* str = "Hello World!";
    printf("axin_string_view test:\n");
    printf("from_parts:");
    axin_string_view_t sv_from_parts = axin_strv_from_parts(str, 5);
    
    return 0;
}
