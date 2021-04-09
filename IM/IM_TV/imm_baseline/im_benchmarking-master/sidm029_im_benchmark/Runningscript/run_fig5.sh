#!/bin/bash
for x in {a..b}
do
	echo "Running experiments for Fig 5"$x
	bash fig5$x".sh"
	echo "Fig 5"$x" done"
done
