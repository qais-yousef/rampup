#!/bin/bash
set -eux

SCRIPTS_DIR=$(realpath $(dirname $0))

sa=$(which sched-analyzer)
if [ -n $sa ]; then
	sudo $sa --util_avg_task --util_est_task --comm periodic &
fi

$SCRIPTS_DIR/periodic

if [ -n $sa ]; then
	sudo pkill -SIGINT sched-analyzer
fi

sa_pp=$(which sched-analyzer-pp)
if [ -n $sa_pp ]; then
	$sa_pp --sched-states periodic --freq --freq-residency \
		--util-avg-running periodic --util-avg-running-residency-abs periodic \
		--util-avg CPU \
		sched-analyzer.perfetto-trace
fi
