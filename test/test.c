#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "b64.h"

#include "test_utils.h"

#define FREE_NULL(ptr) { \
    free((ptr)); \
    (ptr) = NULL; \
}

static uint8_t BYTES_OF_ALL_B64_CHARS[] = {
    0x00, 0x10, 0x83, 0x10, 0x51, 0x87, 0x20, 0x92, 0x8b, 0x30, 0xd3, 0x8f,
    0x41, 0x14, 0x93, 0x51, 0x55, 0x97, 0x61, 0x96, 0x9b, 0x71, 0xd7, 0x9f,
    0x82, 0x18, 0xa3, 0x92, 0x59, 0xa7, 0xa2, 0x9a, 0xab, 0xb2, 0xdb, 0xaf,
    0xc3, 0x1c, 0xb3, 0xd3, 0x5d, 0xb7, 0xe3, 0x9e, 0xbb, 0xf3, 0xdf, 0xbf
};

static char ALL_B64_CHARS[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static char ALL_B64_CHARS_URL_SAFE[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

void test_encoding_all_b64_chars(void) {
    size_t exp_length = strlen(ALL_B64_CHARS);
    size_t length;

    char* encoded_str = b64_std_encode(&length, BYTES_OF_ALL_B64_CHARS, sizeof(BYTES_OF_ALL_B64_CHARS));
    ASSERT_SIZE_EQ(exp_length, length);
    ASSERT_STR_EQ(ALL_B64_CHARS, encoded_str);
    FREE_NULL(encoded_str);

    encoded_str = b64_url_encode(&length, BYTES_OF_ALL_B64_CHARS, sizeof(BYTES_OF_ALL_B64_CHARS));
    ASSERT_SIZE_EQ(exp_length, length);
    ASSERT_STR_EQ(ALL_B64_CHARS_URL_SAFE, encoded_str);
    FREE_NULL(encoded_str);

    encoded_str = b64_mime_encode(&length, BYTES_OF_ALL_B64_CHARS, sizeof(BYTES_OF_ALL_B64_CHARS));
    ASSERT_SIZE_EQ(exp_length, length);
    ASSERT_STR_EQ(ALL_B64_CHARS, encoded_str);
    FREE_NULL(encoded_str);
}

void test_encoding_2bytes_input(void) {
    uint8_t input_bytes[] = { 0xff, 0xff };

    char output_b64_chars[] = "//8=";

    size_t exp_length = strlen(output_b64_chars);
    size_t length;

    char* encoded_str = b64_std_encode(&length, input_bytes, sizeof(input_bytes));
    ASSERT_SIZE_EQ(exp_length, length);
    ASSERT_STR_EQ(output_b64_chars, encoded_str);
    FREE_NULL(encoded_str);

    char output_b64_chars_url_safe[] = "__8";

    encoded_str = b64_url_encode(&length, input_bytes, sizeof(input_bytes));
    ASSERT_SIZE_EQ(strlen(output_b64_chars_url_safe), length);
    ASSERT_STR_EQ(output_b64_chars_url_safe, encoded_str);
    FREE_NULL(encoded_str);

    encoded_str = b64_mime_encode(&length, input_bytes, sizeof(input_bytes));
    ASSERT_SIZE_EQ(exp_length, length);
    ASSERT_STR_EQ(output_b64_chars, encoded_str);
    FREE_NULL(encoded_str);
}

void test_encoding_1byte_input(void) {
    uint8_t input_bytes[] = { 0xff };

    char output_b64_chars[] = "/w==";

    size_t exp_length = strlen(output_b64_chars);
    size_t length;

    char* encoded_str = b64_std_encode(&length, input_bytes, sizeof(input_bytes));
    ASSERT_SIZE_EQ(exp_length, length);
    ASSERT_STR_EQ(output_b64_chars, encoded_str);
    FREE_NULL(encoded_str);

    char output_b64_chars_url_safe[] = "_w";
    encoded_str = b64_url_encode(&length, input_bytes, sizeof(input_bytes));
    ASSERT_SIZE_EQ(strlen(output_b64_chars_url_safe), length);
    ASSERT_STR_EQ(output_b64_chars_url_safe, encoded_str);
    FREE_NULL(encoded_str);

    encoded_str = b64_mime_encode(&length, input_bytes, sizeof(input_bytes));
    ASSERT_SIZE_EQ(exp_length, length);
    ASSERT_STR_EQ(output_b64_chars, encoded_str);
    FREE_NULL(encoded_str);
}

static uint8_t BYTES_OF_B64_CHARS_OVER_76_CHARS[] = {
    0x00, 0x10, 0x83, 0x10, 0x51, 0x87, 0x20, 0x92, 0x8b, 0x30, 0xd3, 0x8f,
    0x41, 0x14, 0x93, 0x51, 0x55, 0x97, 0x61, 0x96, 0x9b, 0x71, 0xd7, 0x9f,
    0x82, 0x18, 0xa3, 0x92, 0x59, 0xa7, 0xa2, 0x9a, 0xab, 0xb2, 0xdb, 0xaf,
    0xc3, 0x1c, 0xb3, 0xd3, 0x5d, 0xb7, 0xe3, 0x9e, 0xbb, 0xf3, 0xdf, 0xbf,
    0x00, 0x10, 0x83, 0x10, 0x51, 0x87, 0x20, 0x92, 0x8b, 0x30, 0xd3, 0x8f
};

static char B64_CHARS_OVER_76_CHARS_WITH_CRLF[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/ABCDEFGHIJKL\x0d\x0a"
    "MNOP";

void test_encoding_to_over_76_chars(void) {
    char B64_CHARS_OVER_76_CHARS[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/ABCDEFGHIJKLMNOP";

    size_t exp_length = strlen(B64_CHARS_OVER_76_CHARS);
    size_t length;

    char* encoded_str = b64_std_encode(&length, BYTES_OF_B64_CHARS_OVER_76_CHARS, sizeof(BYTES_OF_B64_CHARS_OVER_76_CHARS));
    ASSERT_SIZE_EQ(exp_length, length);
    ASSERT_STR_EQ(B64_CHARS_OVER_76_CHARS, encoded_str);
    FREE_NULL(encoded_str);

    char B64_CHARS_OVER_76_CHARS_URL_SAFE[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_ABCDEFGHIJKLMNOP";

    encoded_str= b64_url_encode(&length, BYTES_OF_B64_CHARS_OVER_76_CHARS, sizeof(BYTES_OF_B64_CHARS_OVER_76_CHARS));
    ASSERT_SIZE_EQ(exp_length, length);
    ASSERT_STR_EQ(B64_CHARS_OVER_76_CHARS_URL_SAFE, encoded_str);
    FREE_NULL(encoded_str);

    encoded_str = b64_mime_encode(&length, BYTES_OF_B64_CHARS_OVER_76_CHARS, sizeof(BYTES_OF_B64_CHARS_OVER_76_CHARS));
    ASSERT_SIZE_EQ(strlen(B64_CHARS_OVER_76_CHARS_WITH_CRLF), length);
    ASSERT_STR_EQ(B64_CHARS_OVER_76_CHARS_WITH_CRLF, encoded_str);
    FREE_NULL(encoded_str);
}

void test_mime_encoding_outputs_no_trailing_CRLF(void) {
    static uint8_t BYTES_OF_B64_CHARS_JUST_2LINES[] = {
        0x00, 0x10, 0x83, 0x10, 0x51, 0x87, 0x20, 0x92, 0x8b, 0x30, 0xd3, 0x8f,
        0x41, 0x14, 0x93, 0x51, 0x55, 0x97, 0x61, 0x96, 0x9b, 0x71, 0xd7, 0x9f,
        0x82, 0x18, 0xa3, 0x92, 0x59, 0xa7, 0xa2, 0x9a, 0xab, 0xb2, 0xdb, 0xaf,
        0xc3, 0x1c, 0xb3, 0xd3, 0x5d, 0xb7, 0xe3, 0x9e, 0xbb, 0xf3, 0xdf, 0xbf,
        0x00, 0x10, 0x83, 0x10, 0x51, 0x87, 0x20, 0x92, 0x8b, 0x30, 0xd3, 0x8f,
        0x41, 0x14, 0x93, 0x51, 0x55, 0x97, 0x61, 0x96, 0x9b, 0x71, 0xd7, 0x9f,
        0x82, 0x18, 0xa3, 0x92, 0x59, 0xa7, 0xa2, 0x9a, 0xab, 0xb2, 0xdb, 0xaf,
        0xc3, 0x1c, 0xb3, 0xd3, 0x5d, 0xb7, 0xe3, 0x9e, 0xbb, 0xf3, 0xdf, 0xbf,
        0x00, 0x10, 0x83, 0x10, 0x51, 0x87, 0x20, 0x92, 0x8b, 0x30, 0xd3, 0x8f,
        0x41, 0x14, 0x93, 0x51, 0x55, 0x97
    };

    static char B64_CHARS_JUST_2LINES[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/ABCDEFGHIJKL\x0d\x0a"
        "MNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/ABCDEFGHIJKLMNOPQRSTUVWX";
    size_t length;

    char* encoded_str = b64_mime_encode(&length, BYTES_OF_B64_CHARS_JUST_2LINES, sizeof(BYTES_OF_B64_CHARS_JUST_2LINES));
    ASSERT_SIZE_EQ(strlen(B64_CHARS_JUST_2LINES), length);
    ASSERT_STR_EQ(B64_CHARS_JUST_2LINES, encoded_str);
    FREE_NULL(encoded_str);
}

void test_encoding_fails_when_input_size_is_0(void) {
    uint8_t input_bytes[] = { 0x00 };

    size_t length;

    ASSERT_NULL(b64_std_encode(&length, input_bytes, 0));
    ASSERT_NULL(b64_url_encode(&length, input_bytes, 0));
    ASSERT_NULL(b64_mime_encode(&length, input_bytes, 0));
}


void test_decoding_all_b64_chars(void) {
    size_t size;

    uint8_t* output_bytes = b64_std_decode(&size, ALL_B64_CHARS);
    ASSERT_SIZE_EQ(sizeof(BYTES_OF_ALL_B64_CHARS), size);
    ASSERT_MEM_EQ(BYTES_OF_ALL_B64_CHARS, output_bytes, size);
    FREE_NULL(output_bytes);

    output_bytes = b64_url_decode(&size, ALL_B64_CHARS_URL_SAFE);
    ASSERT_SIZE_EQ(sizeof(BYTES_OF_ALL_B64_CHARS), size);
    ASSERT_MEM_EQ(BYTES_OF_ALL_B64_CHARS, output_bytes, size);
    FREE_NULL(output_bytes);

    output_bytes = b64_mime_decode(&size, ALL_B64_CHARS);
    ASSERT_SIZE_EQ(sizeof(BYTES_OF_ALL_B64_CHARS), size);
    ASSERT_MEM_EQ(BYTES_OF_ALL_B64_CHARS, output_bytes, size);
    FREE_NULL(output_bytes);
}

void test_decoding_remaining_2bytes(void) {
    char input_b64_chars[] = "//8=";

    uint8_t original_bytes[] = { 0xff, 0xff };

    size_t exp_size = sizeof(original_bytes);
    size_t size;

    uint8_t* output_bytes = b64_std_decode(&size, input_b64_chars);
    ASSERT_SIZE_EQ(exp_size, size);
    ASSERT_MEM_EQ(original_bytes, output_bytes, size);
    FREE_NULL(output_bytes);

    char input_b64_chars_url_safe[] = "__8";

    output_bytes = b64_url_decode(&size, input_b64_chars_url_safe);
    ASSERT_SIZE_EQ(exp_size, size);
    ASSERT_MEM_EQ(original_bytes, output_bytes, size);
    FREE_NULL(output_bytes);

    output_bytes = b64_mime_decode(&size, input_b64_chars);
    ASSERT_SIZE_EQ(exp_size, size);
    ASSERT_MEM_EQ(original_bytes, output_bytes, size);
    FREE_NULL(output_bytes);
}

void test_decoding_remaining_1byte(void) {
    char input_b64_chars[] = "/w==";

    uint8_t original_bytes[] = { 0xff };

    size_t exp_size = sizeof(original_bytes);
    size_t size;

    uint8_t* output_bytes = b64_std_decode(&size, input_b64_chars);
    ASSERT_SIZE_EQ(exp_size, size);
    ASSERT_MEM_EQ(original_bytes, output_bytes, size);
    FREE_NULL(output_bytes);

    char input_b64_chars_url_safe[] = "_w";

    output_bytes = b64_url_decode(&size, input_b64_chars_url_safe);
    ASSERT_SIZE_EQ(exp_size, size);
    ASSERT_MEM_EQ(original_bytes, output_bytes, size);
    FREE_NULL(output_bytes);

    output_bytes = b64_mime_decode(&size, input_b64_chars);
    ASSERT_SIZE_EQ(exp_size, size);
    ASSERT_MEM_EQ(original_bytes, output_bytes, size);
    FREE_NULL(output_bytes);
}

void test_mime_decoding_with_multi_line_encoding_chars(void) {
    size_t size;

    uint8_t* output_bytes = b64_mime_decode(&size, B64_CHARS_OVER_76_CHARS_WITH_CRLF);
    ASSERT_SIZE_EQ(sizeof(BYTES_OF_B64_CHARS_OVER_76_CHARS), size);
    ASSERT_MEM_EQ(BYTES_OF_B64_CHARS_OVER_76_CHARS, output_bytes, size);
    FREE_NULL(output_bytes);
}

void test_mime_decoding_with_non_encoding_char(void) {
    char input_b64_chars[] = "/?w==";
    uint8_t original_bytes[] = { 0xff };

    size_t size;

    uint8_t* output_bytes = b64_mime_decode(&size, input_b64_chars);
    ASSERT_SIZE_EQ(sizeof(original_bytes), size);
    ASSERT_MEM_EQ(original_bytes, output_bytes, size);
    FREE_NULL(output_bytes);
}

void test_decoding_fails_when_input_size_is_0(void) {
    char input_b64_chars[] = "";

    size_t size;

    ASSERT_NULL(b64_std_decode(&size, input_b64_chars));
    ASSERT_NULL(b64_url_decode(&size, input_b64_chars));
    ASSERT_NULL(b64_mime_decode(&size, input_b64_chars));
}

void test_decoding_fails_less_than_1byte(void) {
    char input_b64_chars[] = "/===";

    size_t size;

    ASSERT_NULL(b64_std_decode(&size, input_b64_chars));

    char input_b64_chars_url_safe[] = "_";
    ASSERT_NULL(b64_url_decode(&size, input_b64_chars_url_safe));

    ASSERT_NULL(b64_mime_decode(&size, input_b64_chars));
}

void test_decoding_fails_with_non_encoding_char(void) {
    char input_b64_chars[] = "ABC?";

    size_t size;

    ASSERT_NULL(b64_std_decode(&size, input_b64_chars));
    ASSERT_NULL(b64_url_decode(&size, input_b64_chars));
}

int main(void) {
    ADD_TEST_CASE(test_encoding_all_b64_chars);
    ADD_TEST_CASE(test_encoding_2bytes_input);
    ADD_TEST_CASE(test_encoding_1byte_input);
    ADD_TEST_CASE(test_encoding_to_over_76_chars);

    ADD_TEST_CASE(test_mime_encoding_outputs_no_trailing_CRLF);

    ADD_TEST_CASE(test_encoding_fails_when_input_size_is_0);

    ADD_TEST_CASE(test_decoding_all_b64_chars);
    ADD_TEST_CASE(test_decoding_remaining_2bytes);
    ADD_TEST_CASE(test_decoding_remaining_1byte);

    ADD_TEST_CASE(test_mime_decoding_with_multi_line_encoding_chars);
    ADD_TEST_CASE(test_mime_decoding_with_non_encoding_char);

    ADD_TEST_CASE(test_decoding_fails_when_input_size_is_0);
    ADD_TEST_CASE(test_decoding_fails_less_than_1byte);
    ADD_TEST_CASE(test_decoding_fails_with_non_encoding_char);

    run_all_tests();

    return 0;
}
