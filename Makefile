CC     = gcc
CFLAGS = -Wall -Wextra -g
TARGET = minishell
SRCS   = main.c minishell.c

all: $(TARGET)

$(TARGET): $(SRCS) main.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) *.o a.out

.PHONY: all clean
