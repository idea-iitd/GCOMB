#!/bin/bash
#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
execfilecelfpp="../Executables/celfpp"
pathtograph="../Dataset/hepph/graph_lt.inf"
model="LT"
outputcelfpp="../Results/freshRuns/algorithmOutput/fig14f/celfpp"
mkdir -p ../Results/freshRuns/algorithmOutput/fig14f
mkdir -p $outputcelfpp
numiter=10000
execFlow="../Executables/ltFlow"
for mc in 1000 2500 5000 7500 10000
#for mc in 1 #to test use this line instead of above for quick result check
do
	echo "running celfpp" 
	$execfilecelfpp -propModel $model -mcruns $mc -budget 200 -celfPlus 0 -probGraphFile $pathtograph -outdir $outputcelfpp
	echo $execFlow $pathtograph $outputcelfpp"/seeds_CELF_LT_"$mc".txt" 200 1 $numiter 3 $outputcelfpp"/flowst_"$mc".txt" $outputcelfpp"/spread.txt"
	$execFlow $pathtograph $outputcelfpp"/seeds_CELF_LT_"$mc".txt" 200 1 $numiter 3 $outputcelfpp"/flowst_"$mc".txt" $outputcelfpp"/spread.txt"
done
mkdir -p ../Results/freshRuns/processed/fig14f
python ../Codes/process5.py celf $outputcelf"/flowst_" ../Results/freshRuns/processed/fig14f/output.txt
