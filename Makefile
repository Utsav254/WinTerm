CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=gnu99
SRCDIR = src
BINDIR = bin
SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(BINDIR)/%.o, $(SOURCES))
EXECUTABLE = exp

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@

$(BINDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

valg: $(EXECUTABLE)
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(EXECUTABLE)

clean:
	rm -f $(BINDIR)/*.o $(EXECUTABLE)

gdb:
	gdb ./$(EXECUTABLE)

test: test.c
	$(CC) test.c -o test $(CFLAGS)
