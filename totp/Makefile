CC = gcc
override CFLAGS += -Wall -Wextra -fshort-enums -std=gnu17
override LDFLAGS += $(shell pkg-config --libs libcrypto)
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))

TARGET = $(shell basename $(CURDIR))

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

format:
	clang-format -i --style=file $(SRCS) $(wildcard *.h)

clean:
	rm -rf *.o $(TARGET)
