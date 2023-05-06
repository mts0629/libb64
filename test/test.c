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

int main(void) {
    test_encode();

    printf("Test finished\n");

    return 0;
}
