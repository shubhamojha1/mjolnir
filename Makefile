CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
SRCS = main.c tokenizer.c history.c builtins.c exec.c
OBJS = $(SRCS:.c=.o)
TARGET = mjolnir

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
