CC     := gcc
CFLAGS  = -Wall -Wextra -Wpedantic -std=c99 -I$(INC_DIR) -L$(BIN_DIR)
LDFLAGS = -lb64

INC_DIR  := ./include
SRC_DIR  := ./src
TEST_DIR := ./test
BIN_DIR  := ./bin

DEBUG ?= no
ifeq ($(DEBUG), yes)
	CFLAGS += -O0 -g
	CONFIG := debug
	LIB_NAME = libb64d
	LDFLAGS = -lb64d
else
	CFLAGS += -O2
	CONFIG := release
	LIB_NAME = libb64
	LDFLAGS = -lb64
endif

RM := rm -rf

.PHONY: all release debug test lib bindir clean

all: lib

test: $(TEST_DIR)/test.c lib
	$(CC) $< $(CFLAGS) $(LDFLAGS) -o $(BIN_DIR)/test
	$(BIN_DIR)/test

lib: $(SRC_DIR)/b64.c bindir
	$(CC) $< $(CFLAGS) -c -o $(BIN_DIR)/b64.o
	$(AR) rc $(BIN_DIR)/$(LIB_NAME).a $(BIN_DIR)/b64.o

bindir:
	@mkdir -p $(BIN_DIR)

clean:
	$(RM) $(BIN_DIR)
