#!/bin/bash
for x in {a..w}
do
	echo "Running experiments for Fig 14"$x
	bash fig14$x".sh"
	echo "Fig 14"$x" done"
done
