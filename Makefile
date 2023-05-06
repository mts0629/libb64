CC     := gcc
CFLAGS  = -Wall -Wpedantic -std=c99 -I$(INC_DIR) -L$(BIN_DIR)

INC_DIR  := ./include
SRC_DIR  := ./src
TEST_DIR := ./test
BIN_DIR  := ./bin

.PHONY: all test lib bindir clean

all: test

test: $(TEST_DIR)/test.c lib bindir
	$(CC) $< $(CFLAGS) -lb64 -o $(BIN_DIR)/test
	$(BIN_DIR)/test

lib: $(SRC_DIR)/b64.c
	$(CC) $< $(CFLAGS) -c -o $(BIN_DIR)/b64.o
	$(AR) rc $(BIN_DIR)/libb64.a $(BIN_DIR)/b64.o

bindir:
	@mkdir -p $(BIN_DIR)

clean:
	$(RM) $(BIN_DIR)/*
