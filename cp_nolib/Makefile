ASMMODULES = start.asm calls.asm
OBJMODULES = $(ASMMODULES:.asm=.o)

ASMC = nasm
ASMFLAGS = -g -f elf

CC = gcc
CFLAGS = -ggdb -Wall -m32

LD = ld
LDFLAGS = -m elf_i386

%.o: %.asm
	$(ASMC) $(ASMFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

prog: main.o $(OBJMODULES)
	$(LD) $(LDFLAGS) $^ -o $@
