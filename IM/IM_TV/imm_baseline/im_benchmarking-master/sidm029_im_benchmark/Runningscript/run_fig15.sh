#!/bin/bash
for x in {a..o}
do
	echo "Running experiments for Fig 15"$x
	bash fig15$x".sh"
	echo "Fig 15"$x" done"
done
