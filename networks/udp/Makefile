SRCMODULES = inet_port_aton.c
OBJMODULES = $(SRCMODULES:.c=.o)

CC = gcc
CFLAGS = -ggdb -Wall

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

server: server.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

client: client.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCMODULES)
	$(CC) -MM $^ > $@

clean:
	rm -rf deps.mk $(OBJMODULES) client server
