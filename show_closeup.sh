#!/bin/bash
set -eux

inspect() {
	task=$1
	start=$2
	end=$3
	sched-analyzer-pp --freq-task $task --freq-residency-abs-task $task \
		--util-est-running $task --util-avg-running $task \
		--util-est-running-residency-abs $task --util-avg-running-residency-abs $task \
		--sched-states $task \
		--fig-height-tui 20 --ts-start $start --ts-end $end \
		sched-analyzer.perfetto-trace
}

inspect rampup ${1:-1.7} ${2:-2}
inspect periodic ${1:-1} ${2:-2}
