#!/bin/bash
for i in `seq 1 100`;
do
	echo $i
	python process.py $1 $2 $i
done  
