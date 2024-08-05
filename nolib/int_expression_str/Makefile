SRCMODULES = stack.c calculate_expression.c
OBJMODULES = $(SRCMODULES:.c=.o)

CC = gcc
CFLAGS = -ggdb -Wall
CPPFLAGS = -DDEBUG

%.o: %.c %.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

prog: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

-include deps.mk

deps.mk: $(SRCMODULES)
	$(CC) -MM $^ > $@
