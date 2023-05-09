# libb64

## Description

Base64 encoding / decoding with C

## Build

Build static library `bin/libb64.a`

```c
$ make lib
gcc src/b64.c -Wall -Wpedantic -std=c99 -I./include -L./bin -c -o ./bin/b64.o
ar rc ./bin/libb64.a ./bin/b64.o
```

## Test

```c
$ make test
gcc src/b64.c -Wall -Wpedantic -std=c99 -I./include -L./bin -c -o ./bin/b64.o
ar rc ./bin/libb64.a ./bin/b64.o
gcc test/test.c -Wall -Wpedantic -std=c99 -I./include -L./bin -lb64 -o ./bin/test
./bin/test
Test finished
```

## Usage

### Encoding

```c
#include "b64.h"

// Input byte array
uint8_t input_bytes[6] = "ABCDEFG";
// Output string of sufficient byte size
char base64_str[32];

// base64_str is "QUJDREVGRw=="
b64_encode(base64_str, input_bytes, sizeof(input_bytes));
```

### Decoding

```c
#include "b64.h"

// Input base64 string
char base64_str[] = "QUJDREVGRw==";
// Output byte array of sufficient byte size
uint8_t decoded_bytes[32];

// decoded_bytes is "ABCDEFG"
b64_decode(decoded_bytes, base64_str, sizeof(base64_str));
```
