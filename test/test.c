#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "b64.h"

// Test utilities
// Test status
typedef struct TestStatus_tag {
    int num_test_cases;
    int num_fails;
    bool case_was_passed;
} TestStatus;

static TestStatus test_status;

// Update the test status
inline static void add_test_case(void) {
    test_status.num_test_cases++;
    test_status.case_was_passed = true;
}

inline static void count_fail(void) {
    test_status.num_fails++;
    test_status.case_was_passed = false;
}

// Show a status of the current test case
inline static void print_case_status(void) {
    if (test_status.case_was_passed) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
}

// Show the test status
inline static void show_test_status(void) {
    printf("PASSED: %d/%d\n", (test_status.num_test_cases - test_status.num_fails), test_status.num_test_cases);
}

// Test macro
#define RUN_TEST(test_case_func) { \
    add_test_case(); \
    printf("%s ... ", # test_case_func); \
    (test_case_func)(); \
    print_case_status(); \
}

// Assertions to check equality
static bool assert_int_eq(const int expected, const int actual) {
    if (expected != actual) {
        printf("FAIL: expected %d was %d\n", expected, actual);
        return false;
    }
    return true;
}

#define ASSERT_INT_EQ(expected, actual) { \
    if (!assert_int_eq((expected), (actual))) { \
        count_fail(); \
        return; \
    } \
}

static bool assert_str_eq(const char *expected, const char *actual) {
    if (strcmp(expected, actual) != 0) {
        printf("FAIL: expected %s was %s\n", expected, actual);
        return false;
    }
    return true;
}

static bool assert_mem_eq(const uint8_t *expected, const uint8_t *actual, const size_t size) {
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

// Assertion macros
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

static uint8_t BYTES_FOR_ALL_B64_CHARS[] = {
    0x00, 0x10, 0x83,
    0x10, 0x51, 0x87,
    0x20, 0x92, 0x8b,
    0x30, 0xd3, 0x8f,
    0x41, 0x14, 0x93,
    0x51, 0x55, 0x97,
    0x61, 0x96, 0x9b,
    0x71, 0xd7, 0x9f,
    0x82, 0x18, 0xa3,
    0x92, 0x59, 0xa7,
    0xa2, 0x9a, 0xab,
    0xb2, 0xdb, 0xaf,
    0xc3, 0x1c, 0xb3,
    0xd3, 0x5d, 0xb7,
    0xe3, 0x9e, 0xbb,
    0xf3, 0xdf, 0xbf
};

static uint8_t BYTES_FOR_MULTI_LINE_B64_CHARS[] = {
    0x00, 0x10, 0x83,
    0x10, 0x51, 0x87,
    0x20, 0x92, 0x8b,
    0x30, 0xd3, 0x8f,
    0x41, 0x14, 0x93,
    0x51, 0x55, 0x97,
    0x61, 0x96, 0x9b,
    0x71, 0xd7, 0x9f,
    0x82, 0x18, 0xa3,
    0x92, 0x59, 0xa7,
    0xa2, 0x9a, 0xab,
    0xb2, 0xdb, 0xaf,
    0xc3, 0x1c, 0xb3,
    0xd3, 0x5d, 0xb7,
    0xe3, 0x9e, 0xbb,
    0xf3, 0xdf, 0xbf,
    0x00, 0x10, 0x83,
    0x10, 0x51, 0x87,
    0x20, 0x92, 0x8b,
    0x30, 0xd3, 0x8f
};

static uint8_t BYTES_FOR_JUST_2LINES_B64_CHARS[] = {
    0x00, 0x10, 0x83,
    0x10, 0x51, 0x87,
    0x20, 0x92, 0x8b,
    0x30, 0xd3, 0x8f,
    0x41, 0x14, 0x93,
    0x51, 0x55, 0x97,
    0x61, 0x96, 0x9b,
    0x71, 0xd7, 0x9f,
    0x82, 0x18, 0xa3,
    0x92, 0x59, 0xa7,
    0xa2, 0x9a, 0xab,
    0xb2, 0xdb, 0xaf,
    0xc3, 0x1c, 0xb3,
    0xd3, 0x5d, 0xb7,
    0xe3, 0x9e, 0xbb,
    0xf3, 0xdf, 0xbf,
    0x00, 0x10, 0x83,
    0x10, 0x51, 0x87,
    0x20, 0x92, 0x8b,
    0x30, 0xd3, 0x8f,
    0x41, 0x14, 0x93,
    0x51, 0x55, 0x97,
    0x61, 0x96, 0x9b,
    0x71, 0xd7, 0x9f,
    0x82, 0x18, 0xa3,
    0x92, 0x59, 0xa7,
    0xa2, 0x9a, 0xab,
    0xb2, 0xdb, 0xaf,
    0xc3, 0x1c, 0xb3,
    0xd3, 0x5d, 0xb7,
    0xe3, 0x9e, 0xbb,
    0xf3, 0xdf, 0xbf,
    0x00, 0x10, 0x83,
    0x10, 0x51, 0x87,
    0x20, 0x92, 0x8b,
    0x30, 0xd3, 0x8f,
    0x41, 0x14, 0x93,
    0x51, 0x55, 0x97
};

static char STR_OF_ALL_B64_CHARS[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static char STR_OF_ALL_B64_CHARS_URL_SAFE[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

static char STR_OF_MULTI_LINE_B64_CHARS[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/ABCDEFGHIJKL\x0d\x0a"
    "MNOP";

static char STR_OF_JUST_2LINES_B64_CHARS[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/ABCDEFGHIJKL\x0d\x0a"
    "MNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/ABCDEFGHIJKLMNOPQRSTUVWX";

void test_encoding_all_b64_chars(void) {
    char encoded_str[64 + 1] = { '\0' };

    ASSERT_INT_EQ(b64_encode(encoded_str, BYTES_FOR_ALL_B64_CHARS, sizeof(BYTES_FOR_ALL_B64_CHARS)), 64);

    ASSERT_STR_EQ(STR_OF_ALL_B64_CHARS, encoded_str);
}

void test_encoding_2bytes_input(void) {
    uint8_t input_bytes[] = { 0xff, 0xff };
    char b64_str[4 + 1];

    ASSERT_INT_EQ(b64_encode(b64_str, input_bytes, sizeof(input_bytes)), 4);

    ASSERT_STR_EQ("//8=", b64_str);
}

void test_encoding_1byte_input(void) {
    uint8_t input_bytes[] = { 0xff };
    char b64_str[4 + 1];

    ASSERT_INT_EQ(b64_encode(b64_str, input_bytes, sizeof(input_bytes)), 4);

    ASSERT_STR_EQ("/w==", b64_str);
}

void test_url_encoding_all_b64_chars(void) {
    char encoded_str[64 + 1] = { '\0' };

    ASSERT_INT_EQ(b64_url_encode(encoded_str, BYTES_FOR_ALL_B64_CHARS, sizeof(BYTES_FOR_ALL_B64_CHARS)), 64);

    ASSERT_STR_EQ(STR_OF_ALL_B64_CHARS_URL_SAFE, encoded_str);
}

void test_url_encoding_2bytes_input(void) {
    uint8_t input_bytes[] = { 0xff, 0xff };
    char b64_str[3 + 1];

    ASSERT_INT_EQ(b64_url_encode(b64_str, input_bytes, sizeof(input_bytes)), 3);

    ASSERT_STR_EQ("__8", b64_str);
}

void test_url_encoding_1byte_input(void) {
    uint8_t input_bytes[] = { 0xff };
    char b64_str[2 + 1];

    ASSERT_INT_EQ(b64_url_encode(b64_str, input_bytes, sizeof(input_bytes)), 2);

    ASSERT_STR_EQ("_w", b64_str);
}

void test_mime_encoding_with_multi_lines(void) {
    char encoded_str[82 + 1] = { '\0' };

    ASSERT_INT_EQ(b64_mime_encode(encoded_str, BYTES_FOR_MULTI_LINE_B64_CHARS, sizeof(BYTES_FOR_MULTI_LINE_B64_CHARS)), 82);

    ASSERT_STR_EQ(STR_OF_MULTI_LINE_B64_CHARS, encoded_str);
}

void test_mime_encoding_with_just_2lines(void) {
    char encoded_str[154 + 1] = { '\0' };

    ASSERT_INT_EQ(b64_mime_encode(encoded_str, BYTES_FOR_JUST_2LINES_B64_CHARS, sizeof(BYTES_FOR_JUST_2LINES_B64_CHARS)), 154);

    ASSERT_STR_EQ(STR_OF_JUST_2LINES_B64_CHARS, encoded_str);
}


void test_decoding_all_b64_chars(void) {
    uint8_t decoded_bytes[48] = { 0 };

    int size = b64_decode(decoded_bytes, STR_OF_ALL_B64_CHARS);
    ASSERT_INT_EQ(size, 48);

    ASSERT_MEM_EQ(BYTES_FOR_ALL_B64_CHARS, decoded_bytes, size);
}

void test_decoding_remaining_2bytes(void) {
    char b64_str[] = "//8=";
    uint8_t original_bytes[] = { 0xff, 0xff };

    uint8_t decoded_bytes[2];
    int size = b64_decode(decoded_bytes, b64_str);
    ASSERT_INT_EQ(size, 2);

    ASSERT_MEM_EQ(original_bytes, decoded_bytes, size);
}

void test_decoding_remaining_1byte(void) {
    char b64_str[] = "/w==";
    uint8_t original_bytes[] = { 0xff };

    uint8_t decoded_bytes[1];
    int size = b64_decode(decoded_bytes, b64_str);
    ASSERT_INT_EQ(size, 1);

    ASSERT_MEM_EQ(original_bytes, decoded_bytes, size);
}

void test_url_decoding_all_b64_chars(void) {
    uint8_t decoded_bytes[48] = { 0 };

    int size = b64_url_decode(decoded_bytes, STR_OF_ALL_B64_CHARS_URL_SAFE);
    ASSERT_INT_EQ(size, 48);

    ASSERT_MEM_EQ(BYTES_FOR_ALL_B64_CHARS, decoded_bytes, size);
}

void test_url_decoding_remaining_2bytes(void) {
    char b64_str[] = "__8";
    uint8_t original_bytes[] = { 0xff, 0xff };

    uint8_t decoded_bytes[2];
    int size = b64_url_decode(decoded_bytes, b64_str);
    ASSERT_INT_EQ(size, 2);

    ASSERT_MEM_EQ(original_bytes, decoded_bytes, size);
}

void test_url_decoding_remaining_1byte(void) {
    char b64_str[] = "_w";
    uint8_t original_bytes[] = { 0xff };

    uint8_t decoded_bytes[1];
    int size = b64_url_decode(decoded_bytes, b64_str);
    ASSERT_INT_EQ(size, 1);

    ASSERT_MEM_EQ(original_bytes, decoded_bytes, size);
}

void test_mime_decoding_with_multi_lines(void) {
    uint8_t decoded_bytes[60] = { 0 };

    int size = b64_mime_decode(decoded_bytes, STR_OF_MULTI_LINE_B64_CHARS);
    ASSERT_INT_EQ(size, 60);

    ASSERT_MEM_EQ(BYTES_FOR_MULTI_LINE_B64_CHARS, decoded_bytes, size);
}

void test_mime_decoding_with_just_2lines(void) {
    uint8_t decoded_bytes[114] = { 0 };

    int size = b64_mime_decode(decoded_bytes, STR_OF_JUST_2LINES_B64_CHARS);
    ASSERT_INT_EQ(size, 114);

    ASSERT_MEM_EQ(BYTES_FOR_JUST_2LINES_B64_CHARS, decoded_bytes, size);
}

void test_mime_decoding_with_non_encoding_character(void) {
    char b64_str[] = "/?w==";
    uint8_t original_bytes[] = { 0xff };

    uint8_t decoded_bytes[] = { 0 };

    int size = b64_mime_decode(decoded_bytes, b64_str);
    ASSERT_INT_EQ(size, 1);

    ASSERT_MEM_EQ(original_bytes, decoded_bytes, size);
}

void test_decoding_fails_less_than_1byte(void) {
    char b64_str[] = "/===";

    uint8_t decoded_bytes;
    ASSERT_INT_EQ(b64_decode(&decoded_bytes, b64_str), B64_ERROR_REMAINING_BITS);
}

void test_decoding_fails_by_invalid_string(void) {
    uint8_t decoded_bytes[32] = { 0 };

    ASSERT_INT_EQ(b64_decode(decoded_bytes, "????"), B64_ERROR_INVALID_CHAR);
}

int main(void) {
    RUN_TEST(test_encoding_all_b64_chars);
    RUN_TEST(test_encoding_2bytes_input);
    RUN_TEST(test_encoding_1byte_input);

    RUN_TEST(test_url_encoding_all_b64_chars);
    RUN_TEST(test_url_encoding_2bytes_input);
    RUN_TEST(test_url_encoding_1byte_input);

    RUN_TEST(test_mime_encoding_with_multi_lines);
    RUN_TEST(test_mime_encoding_with_just_2lines);

    RUN_TEST(test_decoding_all_b64_chars);
    RUN_TEST(test_decoding_remaining_2bytes);
    RUN_TEST(test_decoding_remaining_1byte);

    RUN_TEST(test_url_decoding_all_b64_chars);
    RUN_TEST(test_url_decoding_remaining_2bytes);
    RUN_TEST(test_url_decoding_remaining_1byte);

    RUN_TEST(test_mime_decoding_with_multi_lines);
    RUN_TEST(test_mime_decoding_with_just_2lines);
    RUN_TEST(test_mime_decoding_with_non_encoding_character);

    RUN_TEST(test_decoding_fails_less_than_1byte);
    RUN_TEST(test_decoding_fails_by_invalid_string);

    show_test_status();

    return 0;
}
