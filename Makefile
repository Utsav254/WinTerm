CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -Iinclude
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
	valgrind -s --leak-check=full ./$(EXECUTABLE)

clean:
	rm -f $(BINDIR)/*.o $(EXECUTABLE)

gdb:
	gdb ./$(EXECUTABLE)
