#include <stdio.h>
#include <string.h>

#include "b64.h"

#include "test_utils.h"

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
    char encoded_str[sizeof(ALL_B64_CHARS)] = { '\0' };
    size_t exp_length = strlen(ALL_B64_CHARS);

    ASSERT_INT_EQ(exp_length, b64_encode(encoded_str, BYTES_OF_ALL_B64_CHARS, sizeof(BYTES_OF_ALL_B64_CHARS)));
    ASSERT_STR_EQ(ALL_B64_CHARS, encoded_str);

    ASSERT_INT_EQ(exp_length, b64_url_encode(encoded_str, BYTES_OF_ALL_B64_CHARS, sizeof(BYTES_OF_ALL_B64_CHARS)));
    ASSERT_STR_EQ(ALL_B64_CHARS_URL_SAFE, encoded_str);

    ASSERT_INT_EQ(exp_length, b64_mime_encode(encoded_str, BYTES_OF_ALL_B64_CHARS, sizeof(BYTES_OF_ALL_B64_CHARS)));
    ASSERT_STR_EQ(ALL_B64_CHARS, encoded_str);
}

void test_encoding_2bytes_input(void) {
    uint8_t input_bytes[] = { 0xff, 0xff };

    char output_b64_chars[] = "//8=";
    char encoded_str[sizeof(output_b64_chars)];

    size_t exp_length = strlen(output_b64_chars);

    ASSERT_INT_EQ(exp_length, b64_encode(encoded_str, input_bytes, sizeof(input_bytes)));
    ASSERT_STR_EQ(output_b64_chars, encoded_str);

    char output_b64_chars_url_safe[] = "__8";

    ASSERT_INT_EQ(strlen(output_b64_chars_url_safe), b64_url_encode(encoded_str, input_bytes, sizeof(input_bytes)));
    ASSERT_STR_EQ(output_b64_chars_url_safe, encoded_str);

    ASSERT_INT_EQ(exp_length, b64_mime_encode(encoded_str, input_bytes, sizeof(input_bytes)));
    ASSERT_STR_EQ(output_b64_chars, encoded_str);
}

void test_encoding_1byte_input(void) {
    uint8_t input_bytes[] = { 0xff };

    char output_b64_chars[] = "/w==";
    char encoded_str[sizeof(output_b64_chars)];

    size_t exp_length = strlen(output_b64_chars);

    ASSERT_INT_EQ(exp_length, b64_encode(encoded_str, input_bytes, sizeof(input_bytes)));
    ASSERT_STR_EQ(output_b64_chars, encoded_str);

    char output_b64_chars_url_safe[] = "_w";
    ASSERT_INT_EQ(strlen(output_b64_chars_url_safe), b64_url_encode(encoded_str, input_bytes, sizeof(input_bytes)));
    ASSERT_STR_EQ(output_b64_chars_url_safe, encoded_str);

    ASSERT_INT_EQ(exp_length, b64_mime_encode(encoded_str, input_bytes, sizeof(input_bytes)));
    ASSERT_STR_EQ(output_b64_chars, encoded_str);
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
    char encoded_str[sizeof(B64_CHARS_OVER_76_CHARS_WITH_CRLF)] = { '\0' };

    char B64_CHARS_OVER_76_CHARS[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/ABCDEFGHIJKLMNOP";

    size_t exp_len = strlen(B64_CHARS_OVER_76_CHARS);

    ASSERT_INT_EQ(exp_len, b64_encode(encoded_str, BYTES_OF_B64_CHARS_OVER_76_CHARS, sizeof(BYTES_OF_B64_CHARS_OVER_76_CHARS)));
    ASSERT_STR_EQ(B64_CHARS_OVER_76_CHARS, encoded_str);

    char B64_CHARS_OVER_76_CHARS_URL_SAFE[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_ABCDEFGHIJKLMNOP";

    ASSERT_INT_EQ(exp_len, b64_url_encode(encoded_str, BYTES_OF_B64_CHARS_OVER_76_CHARS, sizeof(BYTES_OF_B64_CHARS_OVER_76_CHARS)));
    ASSERT_STR_EQ(B64_CHARS_OVER_76_CHARS_URL_SAFE, encoded_str);

    ASSERT_INT_EQ(strlen(B64_CHARS_OVER_76_CHARS_WITH_CRLF), b64_mime_encode(encoded_str, BYTES_OF_B64_CHARS_OVER_76_CHARS, sizeof(BYTES_OF_B64_CHARS_OVER_76_CHARS)));
    ASSERT_STR_EQ(B64_CHARS_OVER_76_CHARS_WITH_CRLF, encoded_str);
}

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

void test_mime_encoding_with_just_2lines(void) {
    char encoded_str[sizeof(B64_CHARS_JUST_2LINES)] = { '\0' };

    ASSERT_INT_EQ(strlen(B64_CHARS_JUST_2LINES), b64_mime_encode(encoded_str, BYTES_OF_B64_CHARS_JUST_2LINES, sizeof(BYTES_OF_B64_CHARS_JUST_2LINES)));

    ASSERT_STR_EQ(B64_CHARS_JUST_2LINES, encoded_str);
}


void test_decoding_all_b64_chars(void) {
    uint8_t decoded_bytes[sizeof(BYTES_OF_ALL_B64_CHARS)] = { 0 };

    int size = b64_decode(decoded_bytes, ALL_B64_CHARS);
    ASSERT_INT_EQ(sizeof(BYTES_OF_ALL_B64_CHARS), size);
    ASSERT_MEM_EQ(BYTES_OF_ALL_B64_CHARS, decoded_bytes, size);

    size = b64_url_decode(decoded_bytes, ALL_B64_CHARS_URL_SAFE);
    ASSERT_INT_EQ(sizeof(BYTES_OF_ALL_B64_CHARS), size);
    ASSERT_MEM_EQ(BYTES_OF_ALL_B64_CHARS, decoded_bytes, size);

    size = b64_decode(decoded_bytes, ALL_B64_CHARS);
    ASSERT_INT_EQ(sizeof(BYTES_OF_ALL_B64_CHARS), size);
    ASSERT_MEM_EQ(BYTES_OF_ALL_B64_CHARS, decoded_bytes, size);

    size = b64_mime_decode(decoded_bytes, ALL_B64_CHARS);
    ASSERT_INT_EQ(sizeof(BYTES_OF_ALL_B64_CHARS), size);
    ASSERT_MEM_EQ(BYTES_OF_ALL_B64_CHARS, decoded_bytes, size);
}

void test_decoding_remaining_2bytes(void) {
    char encoded_str[] = "//8=";
    uint8_t original_bytes[] = { 0xff, 0xff };

    uint8_t decoded_bytes[2];
    int size = b64_decode(decoded_bytes, encoded_str);
    ASSERT_INT_EQ(2, size);
    ASSERT_MEM_EQ(original_bytes, decoded_bytes, size);
}

void test_decoding_remaining_1byte(void) {
    char encoded_str[] = "/w==";
    uint8_t original_bytes[] = { 0xff };

    uint8_t decoded_bytes[1];
    int size = b64_decode(decoded_bytes, encoded_str);

    ASSERT_INT_EQ(1, size);
    ASSERT_MEM_EQ(original_bytes, decoded_bytes, size);
}

void test_url_decoding_remaining_2bytes(void) {
    char encoded_str[] = "__8";
    uint8_t original_bytes[] = { 0xff, 0xff };

    uint8_t decoded_bytes[2];
    int size = b64_url_decode(decoded_bytes, encoded_str);
    ASSERT_INT_EQ(2, size);

    ASSERT_MEM_EQ(original_bytes, decoded_bytes, size);
}

void test_url_decoding_remaining_1byte(void) {
    char encoded_str[] = "_w";
    uint8_t original_bytes[] = { 0xff };

    uint8_t decoded_bytes[1];
    int size = b64_url_decode(decoded_bytes, encoded_str);
    ASSERT_INT_EQ(1, size);

    ASSERT_MEM_EQ(original_bytes, decoded_bytes, size);
}

void test_mime_decoding_with_multi_lines(void) {
    uint8_t decoded_bytes[60] = { 0 };

    int size = b64_mime_decode(decoded_bytes, B64_CHARS_OVER_76_CHARS_WITH_CRLF);
    ASSERT_INT_EQ(60, size);

    ASSERT_MEM_EQ(BYTES_OF_B64_CHARS_OVER_76_CHARS, decoded_bytes, size);
}

void test_mime_decoding_with_just_2lines(void) {
    uint8_t decoded_bytes[114] = { 0 };

    int size = b64_mime_decode(decoded_bytes, B64_CHARS_JUST_2LINES);
    ASSERT_INT_EQ(114, size);

    ASSERT_MEM_EQ(BYTES_OF_B64_CHARS_JUST_2LINES, decoded_bytes, size);
}

void test_mime_decoding_with_non_encoding_character(void) {
    char encoded_str[] = "/?w==";
    uint8_t original_bytes[] = { 0xff };

    uint8_t decoded_bytes[] = { 0 };

    int size = b64_mime_decode(decoded_bytes, encoded_str);
    ASSERT_INT_EQ(1, size);

    ASSERT_MEM_EQ(original_bytes, decoded_bytes, size);
}

void test_decoding_fails_less_than_1byte(void) {
    char encoded_str[] = "/===";

    uint8_t decoded_bytes;
    ASSERT_INT_EQ(B64_ERROR_REMAINING_BITS, b64_decode(&decoded_bytes, encoded_str));
}

void test_decoding_fails_by_invalid_string(void) {
    uint8_t decoded_bytes[32] = { 0 };

    ASSERT_INT_EQ(B64_ERROR_INVALID_CHAR, b64_decode(decoded_bytes, "????"));
}

int main(void) {
    ADD_TEST_CASE(test_encoding_all_b64_chars);
    ADD_TEST_CASE(test_encoding_2bytes_input);
    ADD_TEST_CASE(test_encoding_1byte_input);
    ADD_TEST_CASE(test_encoding_to_over_76_chars);

    ADD_TEST_CASE(test_mime_encoding_with_just_2lines);

    ADD_TEST_CASE(test_decoding_all_b64_chars);
    ADD_TEST_CASE(test_decoding_remaining_2bytes);
    ADD_TEST_CASE(test_decoding_remaining_1byte);

    ADD_TEST_CASE(test_url_decoding_remaining_2bytes);
    ADD_TEST_CASE(test_url_decoding_remaining_1byte);

    ADD_TEST_CASE(test_mime_decoding_with_multi_lines);
    ADD_TEST_CASE(test_mime_decoding_with_just_2lines);
    ADD_TEST_CASE(test_mime_decoding_with_non_encoding_character);

    ADD_TEST_CASE(test_decoding_fails_less_than_1byte);
    ADD_TEST_CASE(test_decoding_fails_by_invalid_string);

    run_all_tests();

    return 0;
}
