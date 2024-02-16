#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv)
{
	int volatile i = 0;
	int sleeptime = 1;
	int sleepfor = 10000; // 10ms
	int totalsleep = sleeptime * 1000000;

	fprintf(stdout, "sleeping for %d seconds\n", sleeptime);

	while(totalsleep > 0) {
		i = 0;
		while(i++ < 10);
		usleep(sleepfor);
		totalsleep -= sleepfor;
	}

	fprintf(stdout, "looping\n") ;

	i = 0;
	while(++i);

	fprintf(stdout, "done\n", i);

	return 0;
}
