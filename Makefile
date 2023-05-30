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
	LIB_NAME = libb64d.a
	LDFLAGS = -lb64d
else
	CFLAGS += -O2
	CONFIG := release
	LIB_NAME = libb64.a
	LDFLAGS = -lb64
endif

BUILD_DIR := $(BIN_DIR)/$(CONFIG)
TARGET_LIB := $(BUILD_DIR)/$(LIB_NAME)
TARGET_TEST := $(BUILD_DIR)/test

RM := rm -rf

.PHONY: all release debug test lib make_dir clean

all: lib

test: $(TEST_DIR)/test.c lib
	$(CC) $< $(CFLAGS) $(LDFLAGS) -o $(TARGET_TEST)
	$(TARGET_TEST)

lib: $(SRC_DIR)/b64.c make_dir
	$(CC) $< $(CFLAGS) -c -o $(BIN_DIR)/b64.o
	$(AR) rc $(TARGET_LIB) $(BIN_DIR)/b64.o

make_dir:
	@mkdir -p $(BUILD_DIR)

clean:
	$(RM) $(BIN_DIR)
