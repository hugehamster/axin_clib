#ifndef AXIN_CLIB_TEST_H
#define AXIN_CLIB_TEST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define AXIN_TEST_COLOR_RESET  "\x1b[0m"
#define AXIN_TEST_COLOR_RED    "\x1b[31m"
#define AXIN_TEST_COLOR_GREEN  "\x1b[32m"
#define AXIN_TEST_COLOR_YELLOW "\x1b[33m"
#define AXIN_TEST_COLOR_CYAN   "\x1b[36m"

typedef struct {
    size_t failed_count;
} axin_test_context_t;

typedef void (*axin_test_func)(axin_test_context_t* ctx);

typedef struct {
    const char* name;
    axin_test_func func;
} axin_test_case_t;

#define AXIN_TEST_CASE(func) (axin_test_case_t){ #func, func }

static inline bool axin_test_expect_impl(
    axin_test_context_t* ctx,
    bool passed,
    const char* expr,
    const char* file,
    int line
) {
    if (!passed) {
        ctx->failed_count++;
        printf(AXIN_TEST_COLOR_RED "    [%s:%d]: assertion failed\n"
            AXIN_TEST_COLOR_RESET "        %s\n",
            file, line, expr);
    }
    return passed;
}
#define axin_test_expect(ctx, expr) \
    axin_test_expect_impl(ctx, (expr), #expr, __FILE__, __LINE__)

#define axin_test_require(ctx, expr) do { \
    if (!axin_test_expect((ctx), (expr))) { \
        return; \
    } \
} while(0)

static inline int axin_test_run(
    const axin_test_case_t* cases,
    size_t count
) {
    size_t pass_num = 0;
    size_t fail_num = 0;
    for (size_t i = 0; i < count; i++) {
        printf(AXIN_TEST_COLOR_CYAN "[run]"
            AXIN_TEST_COLOR_RESET " %s (%zu/%zu)\n",
            cases[i].name, i + 1, count);
        axin_test_context_t ctx = { 0 };
        cases[i].func(&ctx);
        if (ctx.failed_count == 0) {
            printf(AXIN_TEST_COLOR_GREEN "[pass]"
                AXIN_TEST_COLOR_RESET " %s\n", cases[i].name);
            pass_num++;
        } else {
            printf(AXIN_TEST_COLOR_RED "[fail]"
                AXIN_TEST_COLOR_RESET " %s\n", cases[i].name);
            fail_num++;
        }
    }
    printf(AXIN_TEST_COLOR_CYAN "total: %zu"
        AXIN_TEST_COLOR_RESET " | ", count);
    if (pass_num == count) printf(AXIN_TEST_COLOR_GREEN);
    else if (pass_num == 0) printf(AXIN_TEST_COLOR_RED);
    else printf(AXIN_TEST_COLOR_YELLOW);
    printf("passed: %zu" AXIN_TEST_COLOR_RESET " | ", pass_num);
    if (fail_num != 0) printf(AXIN_TEST_COLOR_RED);
    else printf(AXIN_TEST_COLOR_GREEN);
    printf("failed: %zu" AXIN_TEST_COLOR_RESET "\n", fail_num);
    if (fail_num != 0) return 1;
    return 0;
}

#endif // AXIN_CLIB_TEST_H
