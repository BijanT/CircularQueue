CC = gcc
CFLAGS = -Wall -std=c11
DEPS = CircularQueue.h
OBJ = main.o CircularQueue.o
ODIR = ./bin

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

CircularQueue: $(OBJ)
	$(CC) $(CFLAGS) -o bin/CircularQueue main.o CircularQueue.o
