/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2024 Qais Yousef */
#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static volatile int counter;

static int run_time_us = 10000; /* 10ms */
static int sleep_time_us = 6000; /* 6ms */
static int total_dur_us = 5 * 1000 * 1000; /* 5s */

/* handle arguments */
const char *argp_program_version = "periodic v0.1";
const char *argp_program_bug_address = "<qyousef@layalina.io>";

static char doc[] =
"Start a periodic task that runs and sleeps for a specified ms period.";

enum pi_test_opts_flags {
	OPT_DUMMY_START = 0x80,

	OPT_RUN,
	OPT_SLEEP,
};

static const struct argp_option options[] = {
	{ "run", OPT_RUN, "TIME_MS", 0, "Duration of RUNNING time in ms. Default 10ms." },
	{ "sleep", OPT_SLEEP, "TIME_MS", 0, "Duration of SLEEP time in ms. Default 6ms." },
	{ 0 },
};

static error_t parse_arg(int key, char *arg, struct argp_state *state)
{
	char *end_ptr;

	switch (key) {
	case OPT_RUN:
		errno = 0;
		run_time_us = strtol(arg, &end_ptr, 0) * 1000;
		if (errno != 0) {
			perror("Unsupported run value\n");
			return errno;
		}
		if (end_ptr == arg) {
			fprintf(stderr, "run: no digits were found\n");
			argp_usage(state);
			return -EINVAL;
		}
		break;
	case OPT_SLEEP:
		errno = 0;
		sleep_time_us = strtol(arg, &end_ptr, 0) * 1000;
		if (errno != 0) {
			perror("Unsupported sleep value\n");
			return errno;
		}
		if (end_ptr == arg) {
			fprintf(stderr, "sleep: no digits were found\n");
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

double get_time_us(void)
{
	return clock() * 1000000/ CLOCKS_PER_SEC;
}

void periodic_loop(void)
{
	double ts_start, ts_diff, ts_total = 0;

	while (1) {
		ts_diff = 0;
		ts_start = get_time_us();
		while (ts_diff < run_time_us) {
			counter++;
			ts_diff = get_time_us() - ts_start;
		}
		ts_total += ts_diff;
		usleep(sleep_time_us);
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
