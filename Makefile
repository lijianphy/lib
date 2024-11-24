CC := gcc
CFLAGS := -Wall -Wextra -O2 -march=native -std=c17 -pedantic

SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=%.o)

TARGETS := test_vec test_khash

.PHONY: all clean test

all: $(OBJS) $(TARGETS)

test_vec: test_vec.o
	$(CC) $(CFLAGS) -g -o $@ $^

test_khash: test_khash.o
	$(CC) $(CFLAGS) -o test_khash test_khash.c

test: $(TARGETS)
	valgrind --leak-check=full --show-leak-kinds=all ./test_vec
	valgrind --leak-check=full --show-leak-kinds=all ./test_khash

%.o: %.c vec.h khash.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGETS) $(OBJS)
