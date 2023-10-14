# libb64

Library for Base64 encoding/decoding

## Requirement

- gcc (C99)
- GNU Make
- GNU Binutils (ar)

## Build

Build static library (`libb64.a`):

```sh
# build/release/libb64.a
$ make
# or
$ make static
```

Build shared library (`libb64.so`):

```sh
# build/release/libb64.so
$ make shared
```

Debug build (`libb64d.***`):

```sh
# build/debug/libb64d.a
$ make DEBUG=yes

# build/debug/libb64d.so
$ make shared DEBUG=yes
```

Build and run test:

```sh
# build/release/test_runner
$ make test
```

Build sample:

```sh
# build/release/***
$ make sample
```

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
    char* base64_str = b64_std_encode(&length, input_bytes, sizeof(input_bytes));
    assert(length == 12);

    // "QUJDREVGRw=="
    printf("%s\n", base64_str);

    free(base64_str);
}
```

- `b64_std_encode`: standard encoding
    - 62nd/63rd encoding character: `+`/`/`
    - Padding: `=`
    - No line breaks
- `b64_url_encode`: URL-safe encoding
    - 62nd/63rd encoding character: `-`/`_`
    - Padding: not used
    - No line breaks
- `b64_mime_decode`: MIME encoding
    - 62nd/63rd encoding character: `+`/`/`
    - Padding: `=`
    - Line break by `CR`+`LF` is inserted every 76 characters in the decoded string

`b64_encode` provide the encoding with the following parameters can be specified:

- 62nd/63rd encoding characters
- Use padding or not
- Line length to insert `CR`+`LF` (no line breaks by 0)

### Decoding

```c
#include "b64.h"

void decode_sample(void) {
    // Input base64 string
    char base64_str[] = "QUJDREVGRw==";

    // Output byte array is allocated dynamically
    size_t size;
    uint8_t* decoded_bytes = b64_std_decode(&size, base64_str);

    // "0x41,0x42,0x43,0x44,0x45,0x46,0x47,"
    for (int i = 0; i < size; ++i) {
        printf("0x%2x,", decoded_bytes);
    }
    putchar('\n');

    free(decoded_bytes);
}
```

- `b64_std_encode`: standard decoding
    - 62nd/63rd encoding character: `+`/`/`
    - Non-encoding characters are not allowed
- `b64_url_encode`: URL-safe decoding
    - 62nd/63rd encoding character: `-`/`_`
    - Non-encoding characters are not allowed
- `b64_mime_decode`: MIME encoding
    - 62nd/63rd encoding character: `+`/`/`
    - Non-encoding characters are discarded

`b64_decode` provide the decoding with the following parameters can be specified:
- 62nd/63rd encoding characters
- Validate the input characters

## Sample

- b64_encoder

    Encode an attached Jpeg image `sample/sakura.jpg` to `sample/encoded.txt`:

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

## License

MIT License
