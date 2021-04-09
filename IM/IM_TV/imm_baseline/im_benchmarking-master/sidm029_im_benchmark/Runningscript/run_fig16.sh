#!/bin/bash
for x in {a..m}
do
	echo "Running experiments for Fig 16"$x
	bash fig16$x".sh"
	echo "Fig 16"$x" done"
done
