SRCMODULES = stack.c
OBJMODULES = $(SRCMODULES:.c=.o)

CC = gcc
CFLAGS = -ggdb -Wall

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

prog: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

-include deps.mk

deps.mk: $(SRCMODULES)
	$(CC) -MM $^ > $@
