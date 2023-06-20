# libb64

## Description

Base64 encoding / decoding with C

## Requirements

- gcc
    - `-std=C99`
- GNU ar
- GNU Make

## Build

Build static library `libb64.a` to `bin/release/libb64.a`:

```c
$ make # or `make lib`
gcc -Wall -Wextra -Wpedantic -std=c99 -I./include -O2 -c src/b64.c -o bin/release/src/b64.o
ar rc bin/release/libb64.a bin/release/src/b64.o
```

A library with debug information (`bin/debug/libb64d.a`) can be built with: `make DEBUG=yes`.

## Test

Run test cases in `test/test.c` (`bin/release/test_runner`):

```c
$ make test
gcc -Wall -Wextra -Wpedantic -std=c99 -I./include -O2 -c test/test.c -o bin/release/test/test.o
ar rc bin/release/libb64.a bin/release/src/b64.o
gcc -Wall -Wextra -Wpedantic -std=c99 -I./include -O2 bin/release/test/test.o -L./bin/release -lb64 -o bin/release/test_runner
./bin/release/test_runner
test_encoding_all_b64_chars ... finished
test_encoding_input_lacking_1byte ... finished
test_encoding_input_lacking_2bytes ... finished
test_decoding_all_b64_chars ... finished
test_decoding_output_lacking_1byte ... finished
test_decoding_output_lacking_2bytes ... finished
test_decoding_fails_by_invalid_string ... finished
```

Test with debug build options can be with: `make test DEBUG=yes`.

## Usage

### Encoding

```c
#include "b64.h"

void encode_sample(void) {
    // Input byte array
    uint8_t input_bytes[7] = {
        // "ABCDEFG"
        0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47
    };

    // Output string of sufficient byte size, includes a NUL character ('\0')
    char base64_str[12 + 1];

    b64_encode(base64_str, input_bytes, sizeof(input_bytes));

    // "QUJDREVGRw=="
    printf("%s\n", base64_str);
}
```

### Decoding

```c
#include "b64.h"

void decode_sample(void) {
    // Input base64 string
    char base64_str[] = "QUJDREVGRw==";

    // Output byte array of sufficient byte size
    uint8_t decoded_bytes[7];

    b64_decode(decoded_bytes, base64_str, sizeof(base64_str));

    // "0x41,0x42,0x43,0x44,0x45,0x46,0x47,"
    for (int i = 0; i < 7; ++i) {
        printf("0x%2x,", decoded_bytes);
    }
    putchar('\n');
}
```
