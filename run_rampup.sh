#!/bin/bash
set -eux

SCRIPTS_DIR=$(realpath $(dirname $0))

sa=$(which sched-analyzer)
if [ -n $sa ]; then
	sudo $sa --util_avg --util_est_task --comm rampup &
fi

sleep 1

$SCRIPTS_DIR/rampup

if [ -n $sa ]; then
	sudo pkill -SIGINT sched-analyzer
fi

sa_pp=$(which sched-analyzer-pp)
if [ -n $sa_pp ]; then
	$sa_pp --sched-states rampup --freq --freq-residency \
		--util-avg-running rampup --util-avg-running-residency-abs rampup \
		--util-avg CPU \
		sched-analyzer.perfetto-trace
fi
