#!/bin/bash
set -eux

inspect() {
	task=$1
	sched-analyzer-pp --sched-states $task --freq --freq-residency \
		--freq-task $task --freq-residency-abs-task $task \
		--util-avg-running $task --util-avg-running-residency-abs $task \
		--util-avg CPU \
		--util-est-running $task --util-est-running-residency-abs $task \
		sched-analyzer.perfetto-trace
}

inspect rampup ${1:-1.7} ${2:-2}
inspect periodic ${1:-1} ${2:-2}
