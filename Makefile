CC     := gcc
CFLAGS  = -Wall -Wpedantic -std=c99 -I$(INC_DIR) -L$(BIN_DIR)
LDFLAGS = -lb64

INC_DIR  := ./include
SRC_DIR  := ./src
TEST_DIR := ./test
BIN_DIR  := ./bin

LIB_NAME = libb64

.PHONY: all release debug test lib bindir clean

all: release

release: CFLAGS += -O2
release: lib

debug: CFLAGS += -g -O0
debug: LIB_NAME = libb64d
debug: lib

test_debug: debug
test_debug: LDFLAGS = -lb64d
test_debug: test

test: $(TEST_DIR)/test.c lib bindir
	$(CC) $< $(CFLAGS) $(LDFLAGS) -o $(BIN_DIR)/test
	$(BIN_DIR)/test

lib: $(SRC_DIR)/b64.c
	$(CC) $< $(CFLAGS) -c -o $(BIN_DIR)/b64.o
	$(AR) rc $(BIN_DIR)/$(LIB_NAME).a $(BIN_DIR)/b64.o

bindir:
	@mkdir -p $(BIN_DIR)

clean:
	$(RM) $(BIN_DIR)/*
