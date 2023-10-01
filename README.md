# libb64

## Description

Base64 encoding/decoding static library with C

## Requirements

- GCC (C99)
- GNU ar
- GNU Make

## Build

Build static library `libb64.a` at `build/release` by `make` or `make static`.

```sh
$ make # or `make static`
gcc -Wall -Wextra -Wpedantic -std=c99 -Iinclude -O2 -c src/b64.c -o build/release/src/b64.o
ar rc build/release/libb64.a build/release/src/b64.o
```

Build shared library `libb64.so` at `build/release` by `make shared`.

```sh
$ make shared
gcc -Wall -Wextra -Wpedantic -std=c99 -Iinclude -O2 -c src/b64.c -o build/release/src/b64.o
gcc -Wall -Wextra -Wpedantic -std=c99 -Iinclude -O2 -shared -fPIC build/release/src/b64.o -o build/release/libb64.so
```

A library with debug information (`libb64d.***`) can be built with option: `DEBUG=yes` (e.g. `make DEBUG=yes`) at `build/debug`.

## Test

Run test cases in `test/test.c` by `make test` (`build/***/test_runner` is executed).

```sh
$ make test
# Build and run test cases
# ...
./build/release/test_runner
test_encoding_all_b64_chars ... PASSED
test_encoding_2bytes_input ... PASSED
test_encoding_1byte_input ... PASSED
...
```

Also debug information can be added with: `DEBUG=yes`.

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

    // Output string is allocated dynamically
    size_t length;
    char* base64_str = b64_encode(&length, input_bytes, sizeof(input_bytes));
    assert(length == 12);

    // "QUJDREVGRw=="
    printf("%s\n", base64_str);

    free(base64_str);
}
```

### Decoding

```c
#include "b64.h"

void decode_sample(void) {
    // Input base64 string
    char base64_str[] = "QUJDREVGRw==";

    // Output byte array is allocated dynamically
    size_t size;
    uint8_t* decoded_bytes = b64_decode(&size, base64_str);

    // "0x41,0x42,0x43,0x44,0x45,0x46,0x47,"
    for (int i = 0; i < size; ++i) {
        printf("0x%2x,", decoded_bytes);
    }
    putchar('\n');

    free(decoded_bytes);
}
```

### URL-safe encoding

Provide URL-safe encoding / decoding (`-` and `_` are used instead of `+` and `/`, and padding is not used) by `b64_url_encode` and `b64_url_decode`.

### Encoding for MIME

Provide encoding / decoding for MIME (add `CRlF` per 76 charecters, and ignore non-encoded characters) by `b64_mime_encode` and `b64_mime_decode`.

## Sample

Build sample encoder/decoder programs in `sample` by `make sample`.

```sh
$ make sample
# ...

$ ls build/release/sample/
b64_decoder  b64_encoder
```

- b64_encoder

    Encode an attached BMP image `sample/Pepper.bmp` to `sample/encoded.txt`:

    ```sh
    $ ./build/release/sample/b64_encoder ./sample/sakura.jpg ./sample/encoded.txt 
    Base64 encoding of ./sample/sakura.jpg is finished (87269 to 116360 bytes).
    The string is written to './sample/encoded.txt'.
    $ cat output.txt
    /9j/4AAQSkZJRgABAQEASABIAAD/4T9eRXhpZgAATU0AKgAAAAgADAEPAAIAAAAGAAAAngEQAAIA
    ...
    ```

- b64_decoder

    Decode `sample/encoded.txt` to `sample/decoded.bin`:

    ```sh
    $ ./build/release/sample/b64_decoder ./sample/encoded.txt ./sample/decoded.bin 
    Base64 decoding of ./sample/encoded.txt is finished (116360 to 87269 bytes).
    The byte expression is written to './sample/decoded.bin'.

    $ diff ./sample/sakura.jpg ./sample/decoded.bin
    # No differences
    ```
