#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// Update the test status
void add_test_case(void);

void count_fail(void);

// Show a status of the current test case
void print_case_status(void);

// Show the test status
void show_test_status(void);

// Test macro
#define RUN_TEST(test_case_func) { \
    add_test_case(); \
    printf("%s ... ", # test_case_func); \
    (test_case_func)(); \
    print_case_status(); \
}

// Assertion functions to check equality
// Check equality of interger
bool assert_int_eq(const int expected, const int actual);

// Check equality of NULL-terminated string
bool assert_str_eq(const char *expected, const char *actual);

// Check equality of byte array
bool assert_mem_eq(const uint8_t *expected, const uint8_t *actual, const size_t size);

// Assertion macros
#define ASSERT_INT_EQ(expected, actual) { \
    if (!assert_int_eq((expected), (actual))) { \
        count_fail(); \
        return; \
    } \
}

#define ASSERT_STR_EQ(expected, actual) { \
    if (!assert_str_eq((expected), (actual))) { \
        count_fail(); \
        return; \
    } \
}

#define ASSERT_MEM_EQ(expected, actual, size) { \
    if (!assert_mem_eq((expected), (actual), (size))) { \
        count_fail(); \
        return; \
    } \
}

#endif // TEST_UTILS_H
