CC = gcc
CFLAGS = -O2 -Wall -Wextra
LDFLAGS = -lcurl
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))

TARGET = nextbus

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

format:
	clang-format -i --style=file $(wildcard *.c *.h)

clean:
	rm -rf *.o $(TARGET)
