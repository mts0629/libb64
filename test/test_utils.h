#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Max number of the test cases
#define MAX_NUM_TEST_CASES 100

/*************************/
// Test utility functions 
/*************************/
// Add a function as a test case
void add_test_case(void (*test_func)(void), const char* name);

// Run all test cases
void run_all_tests(void);

// Check equality of interger
bool assert_int_eq(const int expected, const int actual);

// Check equality of NULL-terminated string
bool assert_str_eq(const char *expected, const char *actual);

// Check equality of byte array
bool assert_mem_eq(const uint8_t *expected, const uint8_t *actual, const size_t size);

/**********************/
// Test utility macros
/**********************/
// Add a function as a test case by name of the function
#define ADD_TEST_CASE(test_func) { \
    add_test_case((test_func), # test_func); \
}

// Check equality of interger and return when it was different
#define ASSERT_INT_EQ(expected, actual) { \
    if (!assert_int_eq((expected), (actual))) { \
        return; \
    } \
}

// Check equality of NULL-terminated string and return when it was different
#define ASSERT_STR_EQ(expected, actual) { \
    if (!assert_str_eq((expected), (actual))) { \
        return; \
    } \
}

// Check equality of byte array and return when it was different
#define ASSERT_MEM_EQ(expected, actual, size) { \
    if (!assert_mem_eq((expected), (actual), (size))) { \
        return; \
    } \
}

#endif // TEST_UTILS_H
