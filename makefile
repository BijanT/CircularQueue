CC = gcc
CFLAGS = -Wall -std=c99
DEPS = CircularQueue.h
OBJ = main.o CircularQueue.o
ODIR = bin

$(ODIR)/%.o: %.c $(DEPS) | $(ODIR)
	$(CC) $(CFLAGS) -c -o $@ $<

CircularQueue: $(OBJ)
	$(CC) $(CFLAGS) -o bin/CircularQueue main.o CircularQueue.o

$(ODIR):
	mkdir -p $(ODIR)
