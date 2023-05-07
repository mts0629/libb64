#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "b64.h"

void test_encode(void) {
    char input_str[] = "ABCDEFG";
    char encoded_str[32];

    b64_encode(encoded_str, (uint8_t*)input_str, sizeof(input_str));

    assert(strcmp("QUJDREVGRw==", encoded_str) == 0);
}

void test_decode(void) {
    char input_str[] = "QUJDREVGRw==";
    char decoded_str[32] = { 0 };

    b64_decode((uint8_t*)decoded_str, input_str, sizeof(input_str));

    assert(strcmp("ABCDEFG", decoded_str) == 0);
}

int main(void) {
    test_encode();

    test_decode();

    printf("Test finished\n");

    return 0;
}
