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

uint8_t input_bytes[] = "ABCDEFG";
char encoded_str[32]; // Allocate a memory of sufficient byte size

b64_encode(encoded_str, input_bytes, sizeof(input_bytes)); // encoded_str == "QUJDREVGRw=="
```

### Decoding

```c
#include "b64.h"

uint8_t input_string[] = "QUJDREVGRw==";
uint8_t decoded_bytes[32]; // Allocate a memory of sufficient byte size

b64_decode(decoded_bytes, input_string, sizeof(input_string)); // decoded_bytes == "ABCDEFG\0"
```