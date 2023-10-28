CC = gcc
CFLAGS = -Wall

SRCS_SER = server.c 
SRCS_CLI = client.c
HEAD_SER = shared_head.h
HEAD_CLI = shared_head.h
OBJS_SER = $(SRCS_SER:.c=.o)
OBJS_CLI = $(SRCS_CLI:.c=.o)
TARGET_SER = server
TARGET_CLI = client
LIBS_SER = -pthread
LIBS_CLI = -pthread

.PHONY: all clean

all: $(TARGET_SER) $(TARGET_CLI)

$(TARGET_SER): $(SRCS_SER)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS_SER)

$(TARGET_CLI): $(SRCS_CLI)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS_CLI)

clean:
	rm -f $(OBJS_SER) $(OBJS_CLI) $(TARGET_SER) $(TARGET_CLI)