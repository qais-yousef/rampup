CC=gcc

all: rampup periodic

rampup: rampup.c
	$(CROSS_COMPILE)$(CC) rampup.c -o rampup -static

periodic: periodic.c
	$(CROSS_COMPILE)$(CC) periodic.c -o periodic -static

clean:
	rm -f *.o rampup periodic
