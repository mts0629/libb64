INC_DIR  := include
SRC_DIR  := src
TEST_DIR := test
SAMPLE_DIR := sample

CC     := gcc
CFLAGS  = -Wall -Wextra -Wpedantic -std=c99 -I./$(INC_DIR)

DEBUG ?= no
ifeq ($(DEBUG), yes)
	CFLAGS += -O0 -g
	CONFIG := debug
	LIB_NAME = libb64d
	LDFLAGS = -l:libb64d.a
else
	CFLAGS += -O2
	CONFIG := release
	LIB_NAME = libb64
	LDFLAGS = -l:libb64.a
endif

BUILD_DIR := build/$(CONFIG)

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))

TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS := $(addprefix $(BUILD_DIR)/, $(TEST_SRCS:.c=.o))
TARGET_TEST := $(BUILD_DIR)/test_runner

SAMPLE_SRCS := $(wildcard $(SAMPLE_DIR)/*.c)
SAMPLES := $(addprefix $(BUILD_DIR)/, $(SAMPLE_SRCS:.c=))

RM := rm -rf

.PHONY: staic shared test sample clean

all: static

static: $(OBJS)
	$(AR) rc $(BUILD_DIR)/$(LIB_NAME).a $^

shared: $(OBJS)
	$(CC) $(CFLAGS) $^ -shared -fPIC -o $(BUILD_DIR)/$(LIB_NAME).so

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_OBJS) static
	$(CC) $(CFLAGS) $< -L./$(BUILD_DIR) $(LDFLAGS) -o $(TARGET_TEST)
	./$(TARGET_TEST)

$(BUILD_DIR)/%: %.c static
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -L./$(BUILD_DIR) $(LDFLAGS) -o $@

sample: $(SAMPLES)

clean:
	$(RM) $(BUILD_DIR)
