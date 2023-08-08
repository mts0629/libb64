# libb64

## Description

Base64 encoding / decoding with C

## Requirements

- gcc
    - `-std=C99`
- GNU ar
- GNU Make

## Build

Build static library `libb64.a` to `build/release/libb64.a`:

```c
$ make # or `make lib`
gcc -Wall -Wextra -Wpedantic -std=c99 -I./include -O2 -c src/b64.c -o build/release/src/b64.o
ar rc build/release/libb64.a build/release/src/b64.o
```

A library with debug information (`build/debug/libb64d.a`) can be built with: `make DEBUG=yes`.

## Test

Run test cases in `test/test.c` (`build/release/test_runner`):

```c
$ make test
gcc -Wall -Wextra -Wpedantic -std=c99 -I./include -O2 -c test/test.c -o build/release/test/test.o
ar rc build/release/libb64.a build/release/src/b64.o
gcc -Wall -Wextra -Wpedantic -std=c99 -I./include -O2 build/release/test/test.o -L./build/release -lb64 -o build/release/test_runner
./build/release/test_runner
test_encoding_all_b64_chars ... finished
test_encoding_2bytes_input ... finished
test_encoding_1byte_input ... finished
test_decoding_all_b64_chars ... finished
test_decoding_remaining_2bytes ... finished
test_decoding_remaining_1byte ... finished
test_decoding_fails_less_than_1byte ... finished
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

    assert(b64_encode(base64_str, input_bytes, sizeof(input_bytes)) == 12);

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

    size_t size = b64_decode(decoded_bytes, base64_str);

    // "0x41,0x42,0x43,0x44,0x45,0x46,0x47,"
    for (size_t i = 0; i < size; ++i) {
        printf("0x%2x,", decoded_bytes);
    }
    putchar('\n');
}
```

## Sample

Build sample encoder/decoder programs in `sample`.

```sh
$ make sample
gcc -Wall -Wextra -Wpedantic -std=c99 -I./include -O2 -c src/b64.c -o build/release/src/b64.o
ar rc build/release/libb64.a build/release/src/b64.o
gcc -Wall -Wextra -Wpedantic -std=c99 -I./include -O2 sample/b64_decoder.c -L./build/release -lb64 -o build/release/sample/b64_decoder
gcc -Wall -Wextra -Wpedantic -std=c99 -I./include -O2 sample/b64_encoder.c -L./build/release -lb64 -o build/release/sample/b64_encoder
```

Run encoding:

```sh
$ ./build/release/sample/b64_encoder ./sample/Pepper.bmp ./sample/encoded.txt
Base64 encoding of ./sample/Pepper.bmp is finished (196662 to 262216 bytes).
The string is written to './sample/encoded.txt'.
$ cat output.txt
k02AAMAAAAAADYAAAAoAAAAAAEAAAABAAABABgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABcMHN+L3KAImd4KpKUWL26VbG3NbW5P8DGWMjEXs3HUsrDIU8rJUdHGWdDMU9LK
...
```

Run decoding:

```sh
$ ./build/release/sample/b64_decoder ./sample/encoded.txt ./sample/decoded.bin
Base64 decoding of ./sample/encoded.txt is finished (262216 to 196662 bytes).
The byte expression is written to './sample/decoded.bin'.

$ diff ./sample/Pepper.bmp ./sample/decoded.bin  # Same
```
