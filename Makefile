CC := gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99 -I$(INC_DIR)

DEBUG ?= no
ifeq ($(DEBUG), yes)
	CFLAGS += -O0 -g
	CONFIG := debug
	LIB_NAME := libb64d
else
	CFLAGS += -O2
	CONFIG := release
	LIB_NAME := libb64
endif

INC_DIR := include
SRC_DIR := src
TEST_DIR := test
SAMPLE_DIR := sample

BUILD_DIR := build/$(CONFIG)

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))

TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS := $(addprefix $(BUILD_DIR)/, $(TEST_SRCS:.c=.o))
TARGET_TEST := $(BUILD_DIR)/test_runner

SAMPLE_SRCS := $(wildcard $(SAMPLE_DIR)/*.c)
SAMPLES := $(addprefix $(BUILD_DIR)/, $(SAMPLE_SRCS:.c=))

STATIC_LIB = $(BUILD_DIR)/$(LIB_NAME).a
SHARED_LIB = $(BUILD_DIR)/$(LIB_NAME).so

LDFLAGS = -l:$(STATIC_LIB)

RM := rm -rf

.PHONY: static shared test sample clean

all: static

$(STATIC_LIB): $(OBJS)
	$(AR) rc $@ $^

static: $(STATIC_LIB)

$(SHARED_LIB): $(SRCS)
	$(CC) $(CFLAGS) -shared -fPIC $^ -o $@

shared: $(SHARED_LIB)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_TEST): $(TEST_OBJS) $(STATIC_LIB)
	$(CC) $(CFLAGS) $^ -I$(TEST_DIR) -L$(BUILD_DIR) -o $(TARGET_TEST)

test: $(STATIC_LIB) $(TARGET_TEST)
	./$(TARGET_TEST)

$(BUILD_DIR)/%: %.c $(STATIC_LIB)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -L$(BUILD_DIR) -o $@

sample: $(SAMPLES)

clean:
	$(RM) $(BUILD_DIR)
