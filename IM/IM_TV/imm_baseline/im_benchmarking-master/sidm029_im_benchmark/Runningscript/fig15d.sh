#!/bin/bash
execfileeasyim="../Executables/easyimIC"
pathtograph="../Dataset/dblp/graph_ic.inf"
model="IC"
outputeasyim="../Results/freshRuns/algorithmOutput/fig15d/easyim"
mkdir -p ../Results/freshRuns/algorithmOutput/fig15d
mkdir -p $outputeasyim

execFlowSeeds="../Executables/icFlowSeeds"
flowiter=10000

for iter in 1 2 5 10 25 50 75 100 250 500 750 1000
#for iter in 1
do
	for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
	do
		$execfileeasyim $pathtograph 2 $seeds 1 $iter 3 $outputeasyim
		
		echo $execFlowSeeds $pathtograph $outputeasyim"/seeds_seeds="$seeds"_iter="$iter"_depth=3.txt" $seeds 1 $flowiter 3 $outputeasyim"/flowstat_"$iter".txt" $outputeasyim"/spread.txt"
		$execFlowSeeds $pathtograph $outputeasyim"/seeds_seeds="$seeds"_iter="$iter"_depth=3.txt" $seeds 1 $flowiter 3 $outputeasyim"/flowstat_"$iter".txt" $outputeasyim"/spread.txt"
	done
done
mkdir -p ../Results/freshRuns/processed/fig15d

python ../Codes/convergenceCheck.py easyim $outputeasyim"/flowstat_1000.txt_300" $outputcelf"/flowstat_"  ../Results/freshRuns/processed/fig15d/easyim.txt



