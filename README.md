# libb64

## Description

Base64 encoding / decoding with C

## Requirements

- gcc (C99)
- ar
- GNU Make

## Build

Build static library `libb64.a` to `bin/libb64.a`:

```c
$ make # or `make lib`
cc src/b64.c -Wall -Wpedantic -std=c99 -I./include -L./bin -O2 -c -o ./bin/b64.o
ar rc ./bin/libb64.a ./bin/b64.o
```

A library with debug information (`libb64d.a`) can be built with: `make debug`.

## Test

Run test cases in `test/test.c`:

```c
$ make test
gcc src/b64.c -Wall -Wpedantic -std=c99 -I./include -L./bin -c -o ./bin/b64.o
ar rc ./bin/libb64.a ./bin/b64.o
gcc test/test.c -Wall -Wpedantic -std=c99 -I./include -L./bin -lb64 -o ./bin/test
./bin/test
test_encoding_all_b64_chars ... finished
test_encoding_input_lacking_1byte ... finished
test_encoding_input_lacking_2bytes ... finished
test_decoding_all_b64_chars ... finished
test_decoding_output_lacking_1byte ... finished
test_decoding_output_lacking_2bytes ... finished
test_decoding_fails_by_invalid_string ... finished
```

Test with debug build options can be with: `make test_debug`.

## Usage

### Encoding

```c
#include "b64.h"

// Input byte array
uint8_t input_bytes[7] = {
    // "ABCDEFG"
    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47
};
// Output string of sufficient byte size, includes a NUL character ('\0')
char base64_str[12 + 1];

// base64_str is "QUJDREVGRw=="
b64_encode(base64_str, input_bytes, sizeof(input_bytes));
```

### Decoding

```c
#include "b64.h"

// Input base64 string
char base64_str[] = "QUJDREVGRw==";
// Output byte array of sufficient byte size
uint8_t decoded_bytes[7];

// decoded_bytes is {0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47} ("ABCDEFG")
b64_decode(decoded_bytes, base64_str, sizeof(base64_str));
```
