#!/bin/bash
execfileeasyim="../Executables/easyimLT"
pathtograph="../Dataset/hepph/graph_lt.inf"
model="LT"
outputeasyim="../Results/freshRuns/algorithmOutput/fig14s/easyim"
mkdir -p ../Results/freshRuns/algorithmOutput/fig14s
mkdir -p $outputeasyim

execFlowSeeds="../Executables/ltFlowSeeds"
flowiter=10000
#flowiter=400 #For testing purposes. Use the above line for actual runs!

for iter in 1 2 5 10 25 50 75 100 250 500 750 1000
#for iter in 1
do
	for seeds in 1 50 100 150 200
	do
		$execfileeasyim $pathtograph 2 $seeds 1 $iter 3 $outputeasyim
		
		echo $execFlowSeeds $pathtograph $outputeasyim"/seeds_seeds="$seeds"_iter="$iter"_depth=3.txt" $seeds 1 $flowiter 3 $outputeasyim"/flowstat_"$iter".txt" $outputeasyim"/spread.txt"
		$execFlowSeeds $pathtograph $outputeasyim"/seeds_seeds="$seeds"_iter="$iter"_depth=3.txt" $seeds 1 $flowiter 3 $outputeasyim"/flowstat_"$iter".txt" $outputeasyim"/spread.txt"
	done
done
mkdir -p ../Results/freshRuns/processed/fig14s
python ../Codes/process5.py easyim $outputeasyim"/flowstat_" ../Results/freshRuns/processed/fig14s/output.txt
