CC=gcc

rampup: rampup.c
	$(CROSS_COMPILE)$(CC) rampup.c -o rampup -static

all: rampup

clean:
	rm -f *.o rampup
