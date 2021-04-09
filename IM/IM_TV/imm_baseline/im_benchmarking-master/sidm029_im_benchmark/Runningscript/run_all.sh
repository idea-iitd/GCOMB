#!/bin/bash
echo "Preparing the directory structure before running the codes"
bash createDirectoryStructure.sh
echo "Preparation done"

for num in 1 4 5 9 10 12 14 15 16
do
	echo "Running experiments for Fig "$num
	bash fig$num".sh"
	echo "Fig "$num" done"
done
