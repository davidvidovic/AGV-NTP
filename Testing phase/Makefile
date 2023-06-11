CC := gcc
CFLAGS := -Wall -Wextra
LDFLAGS := -lwiringPi -lwiringPiDev

# Name of the target executable
TARGET := AGV

# Source files
SRCS := main.c

# Object files
OBJS := $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
