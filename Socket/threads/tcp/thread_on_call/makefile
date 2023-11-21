CC = gcc
CFLAGS = -Wall

SRCS_SER = server.c
SRCS_CLI = client.c
HEAD_SER = shared_params.h
HEAD_CLI = shared_params.h
OBJS_SER = $(SRCS_SER:.c=.o)
OBJS_CLI = $(SRCS_CLI:.c=.o)
TARGET_SER = server
TARGET_CLI = client
LIBS_SER = -lc -lpthread
LIBS_CLI = -lc -lpthread

.PHONY: all clean

all: $(TARGET_SER) $(TARGET_CLI)

$(TARGET_SER): $(SRCS_SER)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS_SER)

$(TARGET_CLI): $(SRCS_CLI)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS_CLI)

clean:
	rm -f $(OBJS_SER) $(OBJS_CLI) $(TARGET_SER) $(TARGET_CLI)