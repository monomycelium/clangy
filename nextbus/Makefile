CC = gcc
CFLAGS = -O2 -mcpu=native -Wall -Wextra
LDFLAGS = -O2 -mcpu=native -Wall -Wextra $(shell pkg-config --libs --static jansson libcurl)

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
