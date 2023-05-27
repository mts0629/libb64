#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "b64.h"

// Test utility macro
#define ASSERT_STR_EQ(expected, actual) assert(strcmp((expected), (actual)) == 0)
#define ASSERT_MEM_EQ(expected, actual, size) assert(memcmp((expected), (actual), (size)) == 0)

#define RUN_TEST(test_case_func) { \
    printf("%s\n", # test_case_func); \
    (test_case_func)(); \
}

static uint8_t ORIGINAL_BYTES[] = "ABCDEFG";
static char ENCODED_CHARS[] = "QUJDREVGRw==";

void test_encoding(void) {
    char encoded_str[32] = { '\0' };

    b64_encode(encoded_str, ORIGINAL_BYTES, sizeof(ORIGINAL_BYTES) - 1);

    ASSERT_STR_EQ(ENCODED_CHARS, encoded_str);
}

void test_decoding(void) {
    uint8_t decoded_bytes[32] = { 0 };

    assert(b64_decode(decoded_bytes, ENCODED_CHARS, sizeof(ENCODED_CHARS)) == 0);

    ASSERT_MEM_EQ(ORIGINAL_BYTES, decoded_bytes, sizeof(ORIGINAL_BYTES));
}

void test_decoding_fails_by_invalid_string(void) {
    uint8_t decoded_bytes[32] = { 0 };

    assert(b64_decode(decoded_bytes, "????", sizeof("????")) == 1);
}

int main(void) {
    RUN_TEST(test_encoding);

    RUN_TEST(test_decoding);

    RUN_TEST(test_decoding_fails_by_invalid_string);

    printf("Test finished\n");

    return 0;
}
