CC = zig cc
override CFLAGS += -O3 -Wall -Wextra
override LDFLAGS += -flto
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))

TARGET = $(shell basename $(CURDIR))

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

format:
	clang-format -i --style=file $(wildcard *.c *.h)

clean:
	rm -rf *.o $(TARGET)
