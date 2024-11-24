CC = gcc
CFLAGS = -Wall -Wextra -O2 -march=native -std=c17 -pedantic

BUILD_DIR = build
SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

TARGETS = vec_test

# Create variables for specific object files with BUILD_DIR prefix
VEC_TEST_OBJS = $(addprefix $(BUILD_DIR)/, vec_test.o)

.PHONY: all clean

all: $(BUILD_DIR) $(OBJS) $(TARGETS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

vec_test: $(VEC_TEST_OBJS)
	$(CC) $(CFLAGS) -g -o $@ $^
	valgrind --leak-check=full --show-leak-kinds=all ./vec_test

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGETS)
