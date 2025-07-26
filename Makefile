CC = gcc
CFLAGS = 

SERVER_SRCS := server.c shared.c
SERVER_OBJS := $(SERVER_SRCS:.c=.o)
SERVER_TARGET = cchat_server

CLIENT_SRCS := client.c shared.c
CLIENT_OBJS := $(CLIENT_SRCS:.c=.o)
CLIENT_TARGET = cchat_client

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS) $(CLIENT_TARGET) $(SERVER_TARGET)
