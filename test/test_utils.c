#include "test_utils.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Test status
typedef struct TestStatus_tag {
    int num_test_cases;
    int num_fails;
    bool case_was_passed;
} TestStatus;

static TestStatus test_status;

void add_test_case(void) {
    test_status.num_test_cases++;
    test_status.case_was_passed = true;
}

void count_fail(void) {
    test_status.num_fails++;
    test_status.case_was_passed = false;
}

void print_case_status(void) {
    if (test_status.case_was_passed) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
}

void show_test_status(void) {
    printf("PASSED: %d/%d\n", (test_status.num_test_cases - test_status.num_fails), test_status.num_test_cases);
}

bool assert_int_eq(const int expected, const int actual) {
    if (expected != actual) {
        printf("FAIL: expected %d was %d\n", expected, actual);
        return false;
    }
    return true;
}

bool assert_str_eq(const char *expected, const char *actual) {
    if (strcmp(expected, actual) != 0) {
        printf("FAIL: expected %s was %s\n", expected, actual);
        return false;
    }
    return true;
}

bool assert_mem_eq(const uint8_t *expected, const uint8_t *actual, const size_t size) {
    if (memcmp(expected, actual, size) != 0) {
        printf("FAIL: expected 0x");
        for (size_t i = 0; i < size; ++i) {
            printf("%02x", expected[i]);
        }
        printf(" was 0x");
        for (size_t i = 0; i < size; ++i) {
            printf("%02x", actual[i]);
        }
        printf("\n");
        return false;
    }
    return true;
}
