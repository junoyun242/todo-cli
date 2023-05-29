CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lsqlite3

SRCS = db.c cmd.c main.c
OBJS = $(addprefix output/,$(SRCS:.c=.o))
TARGET = output/program

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

output/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
