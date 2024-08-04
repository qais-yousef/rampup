#!/bin/bash
set -eux

start=${1:-0}
end=${2:-1000}

inspect() {
	task=$1
	sched-analyzer-pp --freq-task $task --freq-residency-abs-task $task \
		--util-est-running $task --util-avg-running $task \
		--util-est-running-residency-abs $task --util-avg-running-residency-abs $task \
		--sched-states $task \
		--fig-height-tui 20 --ts-start $start --ts-end $end \
		sched-analyzer.perfetto-trace
}

inspect rampup
inspect periodic
