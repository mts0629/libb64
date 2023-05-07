#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "b64.h"

static uint8_t ORIGINAL_BYTES[] = "ABCDEFG";
static char ENCODED_CHARS[] = "QUJDREVGRw==";

void test_encode(void) {
    char encoded_str[32] = { '\0' };

    b64_encode(encoded_str, ORIGINAL_BYTES, sizeof(ORIGINAL_BYTES));

    assert(strcmp(ENCODED_CHARS, encoded_str) == 0);
}

void test_decode(void) {
    uint8_t decoded_bytes[32] = { 0 };

    b64_decode(decoded_bytes, ENCODED_CHARS, sizeof(ENCODED_CHARS));

    assert(memcmp(ORIGINAL_BYTES, decoded_bytes, sizeof(ORIGINAL_BYTES)) == 0);
}

int main(void) {
    test_encode();

    test_decode();

    printf("Test finished\n");

    return 0;
}
