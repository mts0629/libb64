INC_DIR  := include
SRC_DIR  := src
TEST_DIR := test
BIN_DIR  := bin

CC     := gcc
CFLAGS  = -Wall -Wextra -Wpedantic -std=c99 -I./$(INC_DIR)

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

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(addprefix $(BUILD_DIR), /$(SRCS:.c=.o))

TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS := $(addprefix $(BUILD_DIR), /$(TEST_SRCS:.c=.o))
TARGET_TEST := $(BUILD_DIR)/test_runner

RM := rm -rf

.PHONY: all test clean

all: $(LIB_NAME)

$(LIB_NAME): $(OBJS)
	$(AR) rc $(BUILD_DIR)/$(LIB_NAME) $^

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_OBJS) $(LIB_NAME)
	$(CC) $(CFLAGS) $< -L./$(BUILD_DIR) $(LDFLAGS) -o $(TARGET_TEST)
	./$(TARGET_TEST)

clean:
	$(RM) $(BIN_DIR)
