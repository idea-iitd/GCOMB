#!/bin/bash
#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
execfilecelf="../Executables/celf"
execfilecelfpp="../Executables/celfpp"
execfileeasyim="../Executables/easyimIC"
pathtograph="../Dataset/hepph/graph_ic.inf"
model="IC"
outputcelf="../Results/freshRuns/algorithmOutput/fig4a/celf"
outputcelfpp="../Results/freshRuns/algorithmOutput/fig4a/celfpp"
outputeasyim="../Results/freshRuns/algorithmOutput/fig4a/easyim"
mkdir -p ../Results/freshRuns/algorithmOutput/fig4a
mkdir -p $outputcelf
mkdir -p $outputcelfpp
mkdir -p $outputeasyim

execFlow="../Executables/icFlow"
execFlowSeeds="../Executables/icFlowSeeds"
flowiter=10000


for iter in 1 2 5 10 25 50 75 100 250 500 750 1000
#for iter in 1
do
	for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
	do
	
		echo $execfileeasyim $pathtograph 2 $seeds 1 $iter 3 $outputeasyim
		$execfileeasyim $pathtograph 2 $seeds 1 $iter 3 $outputeasyim
		
		echo $execFlowSeeds $pathtograph $outputeasyim"/seeds_seeds="$seeds"_iter="$iter"_depth=3.txt" $seeds 1 $flowiter 3 $outputeasyim"/flowstat_"$iter".txt" $outputeasyim"/spread.txt"
		$execFlowSeeds $pathtograph $outputeasyim"/seeds_seeds="$seeds"_iter="$iter"_depth=3.txt" $seeds 1 $flowiter 3 $outputeasyim"/flowstat_"$iter".txt" $outputeasyim"/spread.txt"
	done
done

mkdir -p ../Results/freshRuns/processed/fig4a

python ../Codes/convergenceCheck.py easyim $outputeasyim"/flowstat_1000.txt_300" $outputeasyim"/flowstat_"  ../Results/freshRuns/processed/fig4a/easyim.txt


cp ../Results/Alreadypresent/fig4a/plotnew.p ../Results/freshRuns/processed/fig4a/
cd ../Results/freshRuns/processed/fig4a/
gnuplot plotnew.p
epspdf output.eps
