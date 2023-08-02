#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "b64.h"

// Test utility macro
#define STR_EQ(expected, actual) (strcmp((expected), (actual)) == 0)
#define MEM_EQ(expected, actual, size) (memcmp((expected), (actual), (size)) == 0)

#define RUN_TEST(test_case_func) { \
    printf("%s ... ", # test_case_func); \
    (test_case_func)(); \
    printf("finished\n"); \
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

static char STR_OF_ALL_B64_CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void test_encoding_all_b64_chars(void) {
    char encoded_str[64 + 1] = { '\0' };

    assert(b64_encode(encoded_str, BYTES_FOR_ALL_B64_CHARS, sizeof(BYTES_FOR_ALL_B64_CHARS)) == 64);

    assert(STR_EQ(STR_OF_ALL_B64_CHARS, encoded_str));
}

void test_encoding_input_lacking_1byte(void) {
    uint8_t input_bytes[] = { 0xff, 0xff };
    char b64_str[4 + 1];

    assert(b64_encode(b64_str, input_bytes, sizeof(input_bytes)) == 4);

    assert(STR_EQ("//8=", b64_str));
}

void test_encoding_input_lacking_2bytes(void) {
    uint8_t input_bytes[] = { 0xff };
    char b64_str[4 + 1];

    assert(b64_encode(b64_str, input_bytes, sizeof(input_bytes)) == 4);

    assert(STR_EQ("/w==", b64_str));
}

void test_decoding_all_b64_chars(void) {
    uint8_t decoded_bytes[48] = { 0 };

    size_t size = b64_decode(decoded_bytes, STR_OF_ALL_B64_CHARS, sizeof(STR_OF_ALL_B64_CHARS));
    assert(size == 48);

    assert(MEM_EQ(BYTES_FOR_ALL_B64_CHARS, decoded_bytes, size));
}

void test_decoding_output_lacking_1byte(void) {
    char b64_str[] = "///=";
    uint8_t original_bytes[] = { 0xff, 0xff, 0xc0 };

    uint8_t decoded_bytes[3];
    size_t size = b64_decode(decoded_bytes, b64_str, sizeof(b64_str));
    assert(size == 3);

    assert(MEM_EQ(original_bytes, decoded_bytes, size));
}

void test_decoding_output_lacking_2bytes(void) {
    char b64_str[] = "//=";
    uint8_t original_bytes[] = { 0xff, 0xf0 };

    uint8_t decoded_bytes[2];
    size_t size = b64_decode(decoded_bytes, b64_str, sizeof(b64_str));
    assert(size == 2);

    assert(MEM_EQ(original_bytes, decoded_bytes, size));
}

void test_decoding_fails_by_invalid_string(void) {
    uint8_t decoded_bytes[32] = { 0 };

    assert(b64_decode(decoded_bytes, "????", sizeof("????")) == 0);
}

int main(void) {
    RUN_TEST(test_encoding_all_b64_chars);

    RUN_TEST(test_encoding_input_lacking_1byte);
    RUN_TEST(test_encoding_input_lacking_2bytes);

    RUN_TEST(test_decoding_all_b64_chars);
    RUN_TEST(test_decoding_output_lacking_1byte);
    RUN_TEST(test_decoding_output_lacking_2bytes);

    RUN_TEST(test_decoding_fails_by_invalid_string);

    return 0;
}
