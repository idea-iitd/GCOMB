#!/bin/bash
for x in {a..j}
do
	echo "Running experiments for Fig 4"$x
	bash fig4$x".sh"
	echo "Fig 4"$x" done"
done
