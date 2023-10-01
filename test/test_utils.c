#include "test_utils.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Test case
typedef struct TestCase_tag {
    void (*test_func)(void); // Test case function
    const char *name; // Function name
    bool passed; // Flag shows that this test case has passed
} TestCase;

// Test group 
typedef struct TestGroup_tag {
    int num_cases; // The number of test cases
    TestCase cases[MAX_NUM_TEST_CASES]; // Test cases
} TestGroup;

static TestGroup group;

// Current index of running test case
static int current_case_index;

void add_test_case(void (*test_func)(void), const char* name) {
    if (group.num_cases == MAX_NUM_TEST_CASES) {
        fprintf(stderr, "The number of test cases has reached to the limit: %d\n", MAX_NUM_TEST_CASES);
        exit(1);
    }

    TestCase *tcase = &group.cases[group.num_cases];
    tcase->test_func = test_func;
    tcase->name = name;
    tcase->passed = true;

    group.num_cases++;
}

inline static void show_test_status(void) {
    int passed = 0;
    for (int i = 0; i < group.num_cases; ++i) {
        if (group.cases[i].passed) {
            passed++;
        }
    }

    printf("--------------------\n");
    printf("PASSED: %d/%d\n", passed, group.num_cases);
    printf("--------------------\n");
}

void run_all_tests(void) {
    for (current_case_index = 0; current_case_index < group.num_cases; ++current_case_index) {
        TestCase *tcase = &group.cases[current_case_index];
        printf("%s ... ", tcase->name);
        tcase->test_func();

        if (tcase->passed) {
            printf("PASSED\n");
        } else {
            printf("FAILED\n");
        }
    }

    show_test_status();
}

// Current test case has failed
inline static void set_current_test_case_failed(void) {
    group.cases[current_case_index].passed = false;
}

bool assert_size_eq(const size_t expected, const size_t actual, const char* file, const int line) {
    if (expected != actual) {
        fprintf(stderr, "FAIL: expected %lu was %lu, %s line %d\n", expected, actual, file, line);
        set_current_test_case_failed();
        return false;
    }
    return true;
}

bool assert_str_eq(const char *expected, const char *actual, const char* file, const int line) {
    if (strcmp(expected, actual) != 0) {
        fprintf(stderr, "FAIL: expected %s was %s, %s line %d\n", expected, actual, file, line);
        set_current_test_case_failed();
        return false;
    }
    return true;
}

bool assert_mem_eq(const uint8_t *expected, const uint8_t *actual, const size_t size, const char* file, const int line) {
    if (memcmp(expected, actual, size) != 0) {
        fprintf(stderr, "FAIL: expected 0x");
        for (size_t i = 0; i < size; ++i) {
            printf("%02x", expected[i]);
        }
        fprintf(stderr, " was 0x");
        for (size_t i = 0; i < size; ++i) {
            printf("%02x", actual[i]);
        }
        fprintf(stderr, ", %s line %d\n", file, line);
        set_current_test_case_failed();
        return false;
    }
    return true;
}

bool assert_null(void* ptr, const char* file, const int line) {
    if (ptr != NULL) {
        fprintf(stderr, "FAIL: %p is not NULL, %p, %s line %d", ptr, ptr, file, line);
        set_current_test_case_failed();
        return false;
    }
    return true;
}
