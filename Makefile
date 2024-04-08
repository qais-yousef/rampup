CC=gcc

all:
	$(CROSS_COMPILE)$(CC) rampup.c -o rampup -static
