SRCMODULES = server.c
OBJMODULES = $(SRCMODULES:.c=.o)

CC = gcc
CFLAGS = -ggdb -Wall
LIBS = -lpthread

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

prog: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCMODULES)
	$(CC) -MM $^ > $@

clean:
	rm prog *.o deps.mk
