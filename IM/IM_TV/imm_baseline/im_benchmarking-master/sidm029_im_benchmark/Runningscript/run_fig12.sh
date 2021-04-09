#!/bin/bash
for x in {a..l}
do
	echo "Running experiments for Fig 12"$x
	bash fig12$x".sh"
	echo "Fig 12"$x" done"
done
