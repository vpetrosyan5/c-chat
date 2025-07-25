CC = gcc
CFLAGS = 

SRCS := $(wildcard *.c)

OBJS := $(SRCS:.c=.o)

TARGET = cchat

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
