#!/bin/bash

echo "Running experiments for Fig 9a and 13a"
bash fig9a-13a.sh
echo "Fig 9a and 13a done"

echo "Running experiments for Fig 9b and 13b"
bash fig9b-13b.sh
echo "Fig 9b and 13b done"

for x in {c..e}
do
	echo "Running experiments for Fig 9"$x
	bash fig9$x".sh"
	echo "Fig 9"$x" done"
done
