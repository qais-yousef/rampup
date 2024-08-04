/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2024 Qais Yousef */
#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static volatile int counter;

static int period_us = 16000; /* 16ms */
static int loops = 1000000; /* 1000k */
static int total_dur_us = 5 * 1000 * 1000; /* 5s */

/* handle arguments */
const char *argp_program_version = "periodic v0.1";
const char *argp_program_bug_address = "<qyousef@layalina.io>";

static char doc[] =
"Start a periodic task that runs and loops for Xk times every Yms period.";

enum pi_test_opts_flags {
	OPT_DUMMY_START = 0x80,

	OPT_PERIOD,
	OPT_LOOPS,
};

static const struct argp_option options[] = {
	{ "period", OPT_PERIOD, "TIME_MS", 0, "Period in ms. Default 16ms." },
	{ "loops", OPT_LOOPS, "INT", 0, "Number of loops to execute every wakeup in 'k' units. Default 1000k." },
	{ 0 },
};

static error_t parse_arg(int key, char *arg, struct argp_state *state)
{
	char *end_ptr;

	switch (key) {
	case OPT_PERIOD:
		errno = 0;
		period_us = strtol(arg, &end_ptr, 0) * 1000;
		if (errno != 0) {
			perror("Unsupported period value\n");
			return errno;
		}
		if (end_ptr == arg) {
			fprintf(stderr, "period: no digits were found\n");
			argp_usage(state);
			return -EINVAL;
		}
		break;
	case OPT_LOOPS:
		errno = 0;
		loops = strtol(arg, &end_ptr, 0) * 1000;
		if (errno != 0) {
			perror("Unsupported loops value\n");
			return errno;
		}
		if (end_ptr == arg) {
			fprintf(stderr, "loops: no digits were found\n");
			argp_usage(state);
			return -EINVAL;
		}
		break;
	case ARGP_KEY_ARG:
		argp_usage(state);
		break;
	case ARGP_KEY_END:
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

const struct argp argp = {
	.options = options,
	.parser = parse_arg,
	.doc = doc,
};

int get_time_us(void)
{
	return clock() * 1000000/ CLOCKS_PER_SEC;
}

void periodic_loop(void)
{
	int ts_start, ts_diff, ts_total = 0;

	while (1) {
		counter = loops;
		ts_diff = 0;
		ts_start = get_time_us();
		while (counter--);
		ts_diff = get_time_us() - ts_start;
		if (ts_diff < period_us)
			usleep(period_us - ts_diff);
		else
			fprintf(stderr, "loops is too high, no sleep time\n");
		ts_total += period_us;
		if (ts_total > total_dur_us)
			break;
	}
}

int main(int argc, char *argv[])
{
	int volatile i = 0;
	int sleeptime = 1;
	int sleepfor = 10000; // 10ms
	int totalsleep = sleeptime * 1000000;
	FILE *fd;
	int err;

	err = argp_parse(&argp, argc, argv, 0, NULL, NULL);
	if (err)
		return err;

	fprintf(stdout, "sleeping for %d seconds\n", sleeptime);

	while(totalsleep > 0) {
		i = 0;
		while(i++ < 10);
		usleep(sleepfor);
		totalsleep -= sleepfor;
	}

	fprintf(stdout, "looping\n") ;

	fd = fopen("/sys/kernel/tracing/trace_marker", "w");
	if (fd) {
		fwrite("rampup start\n", sizeof("periodic start\n"), 1, fd);
		fclose(fd);
	}

	periodic_loop();

	fprintf(stdout, "done\n");

	return 0;
}
