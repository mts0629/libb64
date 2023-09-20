#include <stdio.h>
#include <string.h>

#include "b64.h"

#include "test_utils.h"

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
    char encoded_str[sizeof(STR_OF_ALL_B64_CHARS)] = { '\0' };

    ASSERT_INT_EQ(strlen(STR_OF_ALL_B64_CHARS), b64_encode(encoded_str, BYTES_FOR_ALL_B64_CHARS, sizeof(BYTES_FOR_ALL_B64_CHARS)));
    ASSERT_STR_EQ(STR_OF_ALL_B64_CHARS, encoded_str);

    ASSERT_INT_EQ(strlen(STR_OF_ALL_B64_CHARS), b64_url_encode(encoded_str, BYTES_FOR_ALL_B64_CHARS, sizeof(BYTES_FOR_ALL_B64_CHARS)));
    ASSERT_STR_EQ(STR_OF_ALL_B64_CHARS_URL_SAFE, encoded_str);

    ASSERT_INT_EQ(strlen(STR_OF_ALL_B64_CHARS), b64_mime_encode(encoded_str, BYTES_FOR_ALL_B64_CHARS, sizeof(BYTES_FOR_ALL_B64_CHARS)));
    ASSERT_STR_EQ(STR_OF_ALL_B64_CHARS, encoded_str);
}

void test_encoding_2bytes_input(void) {
    uint8_t input_bytes[] = { 0xff, 0xff };
    char b64_str[4 + 1];

    ASSERT_INT_EQ(4, b64_encode(b64_str, input_bytes, sizeof(input_bytes)));

    ASSERT_STR_EQ("//8=", b64_str);
}

void test_encoding_1byte_input(void) {
    uint8_t input_bytes[] = { 0xff };
    char b64_str[4 + 1];

    ASSERT_INT_EQ(4, b64_encode(b64_str, input_bytes, sizeof(input_bytes)));

    ASSERT_STR_EQ("/w==", b64_str);
}

void test_url_encoding_2bytes_input(void) {
    uint8_t input_bytes[] = { 0xff, 0xff };
    char b64_str[3 + 1];

    ASSERT_INT_EQ(3, b64_url_encode(b64_str, input_bytes, sizeof(input_bytes)));

    ASSERT_STR_EQ("__8", b64_str);
}

void test_url_encoding_1byte_input(void) {
    uint8_t input_bytes[] = { 0xff };
    char b64_str[2 + 1];

    ASSERT_INT_EQ(2, b64_url_encode(b64_str, input_bytes, sizeof(input_bytes)));

    ASSERT_STR_EQ("_w", b64_str);
}

void test_encoding_without_crlf(void) {
    static char STR_OVER_76_CHARS[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/ABCDEFGHIJKLMNOP";

    char encoded_str[80 + 1] = { '\0' };

    ASSERT_INT_EQ(80, b64_encode(encoded_str, BYTES_FOR_MULTI_LINE_B64_CHARS, sizeof(BYTES_FOR_MULTI_LINE_B64_CHARS)));
    ASSERT_STR_EQ(STR_OVER_76_CHARS, encoded_str);

    static char STR_OVER_76_CHARS_URL_SAFE[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_ABCDEFGHIJKLMNOP";

    ASSERT_INT_EQ(80, b64_url_encode(encoded_str, BYTES_FOR_MULTI_LINE_B64_CHARS, sizeof(BYTES_FOR_MULTI_LINE_B64_CHARS)));
    ASSERT_STR_EQ(STR_OVER_76_CHARS_URL_SAFE, encoded_str);
}

void test_mime_encoding_with_multi_lines(void) {
    char encoded_str[82 + 1] = { '\0' };

    ASSERT_INT_EQ(82, b64_mime_encode(encoded_str, BYTES_FOR_MULTI_LINE_B64_CHARS, sizeof(BYTES_FOR_MULTI_LINE_B64_CHARS)));

    ASSERT_STR_EQ(STR_OF_MULTI_LINE_B64_CHARS, encoded_str);
}

void test_mime_encoding_with_just_2lines(void) {
    char encoded_str[154 + 1] = { '\0' };

    ASSERT_INT_EQ(154, b64_mime_encode(encoded_str, BYTES_FOR_JUST_2LINES_B64_CHARS, sizeof(BYTES_FOR_JUST_2LINES_B64_CHARS)));

    ASSERT_STR_EQ(STR_OF_JUST_2LINES_B64_CHARS, encoded_str);
}


void test_decoding_all_b64_chars(void) {
    uint8_t decoded_bytes[sizeof(BYTES_FOR_ALL_B64_CHARS)] = { 0 };

    int size = b64_decode(decoded_bytes, STR_OF_ALL_B64_CHARS);
    ASSERT_INT_EQ(sizeof(BYTES_FOR_ALL_B64_CHARS), size);
    ASSERT_MEM_EQ(BYTES_FOR_ALL_B64_CHARS, decoded_bytes, size);

    size = b64_url_decode(decoded_bytes, STR_OF_ALL_B64_CHARS_URL_SAFE);
    ASSERT_INT_EQ(sizeof(BYTES_FOR_ALL_B64_CHARS), size);
    ASSERT_MEM_EQ(BYTES_FOR_ALL_B64_CHARS, decoded_bytes, size);

    size = b64_decode(decoded_bytes, STR_OF_ALL_B64_CHARS);
    ASSERT_INT_EQ(sizeof(BYTES_FOR_ALL_B64_CHARS), size);
    ASSERT_MEM_EQ(BYTES_FOR_ALL_B64_CHARS, decoded_bytes, size);

    size = b64_mime_decode(decoded_bytes, STR_OF_ALL_B64_CHARS);
    ASSERT_INT_EQ(sizeof(BYTES_FOR_ALL_B64_CHARS), size);
    ASSERT_MEM_EQ(BYTES_FOR_ALL_B64_CHARS, decoded_bytes, size);
}

void test_decoding_remaining_2bytes(void) {
    char b64_str[] = "//8=";
    uint8_t original_bytes[] = { 0xff, 0xff };

    uint8_t decoded_bytes[2];
    int size = b64_decode(decoded_bytes, b64_str);
    ASSERT_INT_EQ(2, size);

    ASSERT_MEM_EQ(original_bytes, decoded_bytes, size);
}

void test_decoding_remaining_1byte(void) {
    char b64_str[] = "/w==";
    uint8_t original_bytes[] = { 0xff };

    uint8_t decoded_bytes[1];
    int size = b64_decode(decoded_bytes, b64_str);
    ASSERT_INT_EQ(1, size);

    ASSERT_MEM_EQ(original_bytes, decoded_bytes, size);
}

void test_url_decoding_remaining_2bytes(void) {
    char b64_str[] = "__8";
    uint8_t original_bytes[] = { 0xff, 0xff };

    uint8_t decoded_bytes[2];
    int size = b64_url_decode(decoded_bytes, b64_str);
    ASSERT_INT_EQ(2, size);

    ASSERT_MEM_EQ(original_bytes, decoded_bytes, size);
}

void test_url_decoding_remaining_1byte(void) {
    char b64_str[] = "_w";
    uint8_t original_bytes[] = { 0xff };

    uint8_t decoded_bytes[1];
    int size = b64_url_decode(decoded_bytes, b64_str);
    ASSERT_INT_EQ(1, size);

    ASSERT_MEM_EQ(original_bytes, decoded_bytes, size);
}

void test_mime_decoding_with_multi_lines(void) {
    uint8_t decoded_bytes[60] = { 0 };

    int size = b64_mime_decode(decoded_bytes, STR_OF_MULTI_LINE_B64_CHARS);
    ASSERT_INT_EQ(60, size);

    ASSERT_MEM_EQ(BYTES_FOR_MULTI_LINE_B64_CHARS, decoded_bytes, size);
}

void test_mime_decoding_with_just_2lines(void) {
    uint8_t decoded_bytes[114] = { 0 };

    int size = b64_mime_decode(decoded_bytes, STR_OF_JUST_2LINES_B64_CHARS);
    ASSERT_INT_EQ(114, size);

    ASSERT_MEM_EQ(BYTES_FOR_JUST_2LINES_B64_CHARS, decoded_bytes, size);
}

void test_mime_decoding_with_non_encoding_character(void) {
    char b64_str[] = "/?w==";
    uint8_t original_bytes[] = { 0xff };

    uint8_t decoded_bytes[] = { 0 };

    int size = b64_mime_decode(decoded_bytes, b64_str);
    ASSERT_INT_EQ(1, size);

    ASSERT_MEM_EQ(original_bytes, decoded_bytes, size);
}

void test_decoding_fails_less_than_1byte(void) {
    char b64_str[] = "/===";

    uint8_t decoded_bytes;
    ASSERT_INT_EQ(B64_ERROR_REMAINING_BITS, b64_decode(&decoded_bytes, b64_str));
}

void test_decoding_fails_by_invalid_string(void) {
    uint8_t decoded_bytes[32] = { 0 };

    ASSERT_INT_EQ(B64_ERROR_INVALID_CHAR, b64_decode(decoded_bytes, "????"));
}

int main(void) {
    ADD_TEST_CASE(test_encoding_all_b64_chars);
    ADD_TEST_CASE(test_encoding_2bytes_input);
    ADD_TEST_CASE(test_encoding_1byte_input);

    ADD_TEST_CASE(test_url_encoding_2bytes_input);
    ADD_TEST_CASE(test_url_encoding_1byte_input);

    ADD_TEST_CASE(test_encoding_without_crlf);
    ADD_TEST_CASE(test_mime_encoding_with_multi_lines);
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
