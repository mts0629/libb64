# libb64

## Description

Base64 encoding with C

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