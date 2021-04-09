#!/bin/bash
for x in {a..f}
do
	echo "Running experiments for Fig 10"$x
	bash fig10$x".sh"
	echo "Fig 10"$x" done"
done
