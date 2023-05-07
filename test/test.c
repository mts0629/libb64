#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "b64.h"

static uint8_t ORIGINAL_BYTES[] = "ABCDEFG";
static char ENCODED_CHARS[] = "QUJDREVGRw==";

void test_encoding(void) {
    char encoded_str[32] = { '\0' };

    b64_encode(encoded_str, ORIGINAL_BYTES, sizeof(ORIGINAL_BYTES) - 1);

    printf("%s\n", encoded_str);

    assert(strcmp(ENCODED_CHARS, encoded_str) == 0);
}

void test_decoding(void) {
    uint8_t decoded_bytes[32] = { 0 };

    assert(b64_decode(decoded_bytes, ENCODED_CHARS, sizeof(ENCODED_CHARS)) == 0);

    assert(memcmp(ORIGINAL_BYTES, decoded_bytes, sizeof(ORIGINAL_BYTES)) == 0);
}

void test_decoding_fails_by_invalid_string(void) {
    uint8_t decoded_bytes[32] = { 0 };

    assert(b64_decode(decoded_bytes, "????", sizeof("????")) == 1);
}

int main(void) {
    test_encoding();

    test_decoding();

    test_decoding_fails_by_invalid_string();

    printf("Test finished\n");

    return 0;
}
