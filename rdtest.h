#pragma once

#include <stdio.h>
#include <string.h>

#define RDTEST_PASS 0
#define RDTEST_FAIL 1

typedef int (*RDTestFn)(void);

typedef struct {
    const char* name;
    RDTestFn fn;
} RDTest;

#define rdtest_assert(cond, msg)                                               \
    do {                                                                       \
        if(!(cond)) {                                                          \
            fprintf(stderr, "  FAIL %s:%d: %s\n", __FILE__, __LINE__, (msg));  \
            return RDTEST_FAIL;                                                \
        }                                                                      \
    } while(0)

// clang-format off
#define rdtest_assert_eq(a, b) rdtest_assert((a) == (b), #a " != " #b)
#define rdtest_assert_ne(a, b) rdtest_assert((a) != (b), #a " == " #b)
#define rdtest_assert_true(a) rdtest_assert(!!(a), #a " is not TRUE")
#define rdtest_assert_false(a) rdtest_assert(!(a), #a " is not FALSE")
#define rdtest_assert_null(a) rdtest_assert((a) == NULL, #a " is not NULL")
#define rdtest_assert_notnull(a) rdtest_assert((a) != NULL, #a " is NULL")
#define rdtest_assert_str(a, b) rdtest_assert((a) && (b) && !strcmp((a), (b)), #a " != " #b)
#define rdtest_assert_pass(a) rdtest_assert_eq((a), RDTEST_PASS)
// clang-format on

static inline int rdtest_run(const char* suite, const RDTest* tests) {
    int passed = 0, failed = 0;
    fprintf(stdout, "=== %s ===\n", suite);

    const RDTest* t = tests;

    while(t->name && t->fn) {
        fprintf(stdout, "  [ RUN ] %s\n", t->name);
        int result = t->fn();

        if(result == RDTEST_PASS) {
            fprintf(stdout, "  [ OK  ] %s\n", t->name);
            passed++;
        }
        else {
            fprintf(stdout, "  [FAIL] %s\n", t->name);
            failed++;
        }

        t++;
    }

    fprintf(stdout, "  %d passed, %d failed\n\n", passed, failed);
    return failed == 0 ? RDTEST_PASS : RDTEST_FAIL;
}
